#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
    void ReinkanApp::DrawFrame()
    {
        // can only pass if inFlightFences is [SIGNAL]
        vkWaitForFences(appDevice, 1, &inFlightFences[appCurrentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(appDevice, 
                                                appSwapchain, 
                                                UINT64_MAX, 
                                                imageAvailableSemaphores[appCurrentFrame], 
                                                VK_NULL_HANDLE, 
                                                &imageIndex);
        // After finish on GPU
        // > > > > > > > [SIGNAL] imageAvailableSemaphores[appCurrentFrame]

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            appFramebufferResized = false;
            RecreateSwapchain();
            return;
        }
        else if (result != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        ////////////////////////////////
        //    Update Value per frame
        ////////////////////////////////

        UpdateScanlineUBO(appCurrentFrame);

        // Only reset the fence if we are submitting work
        // [WAIT] inFlightFences[appCurrentFrame] or [UNSIGNAL]
        vkResetFences(appDevice, 1, &inFlightFences[appCurrentFrame]);

        vkResetCommandBuffer(appCommandBuffers[appCurrentFrame], 0);
        RecordCommandBuffer(appCommandBuffers[appCurrentFrame], imageIndex);

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[appCurrentFrame] };
        VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[appCurrentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &appCommandBuffers[appCurrentFrame];
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        // [WAIT] imageAvailableSemaphores[appCurrentFrame]
        if (vkQueueSubmit(appGraphicsQueue, 1, &submitInfo, inFlightFences[appCurrentFrame]) != VK_SUCCESS)
        // After finish on GPU
        // > > > > > > > [SIGNAL] renderFinishedSemaphores[appCurrentFrame]
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkSwapchainKHR swapchains[] = { appSwapchain };

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapchains;
        presentInfo.pImageIndices = &imageIndex;

        // [WAIT] renderFinishedSemaphores[appCurrentFrame]
        vkQueuePresentKHR(appPresentQueue, &presentInfo);
        // > > > > > > > [SIGNAL] inFlightFences[appCurrentFrame]

        appCurrentFrame = (appCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    /*
    * To interface this record command
    * - Interface how the pipeline construct
    * - Interface hot to let use create descriptor set easily
    * - Pass in pipeline and descriptor set
    */
    void ReinkanApp::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
        {
            VkRenderPassBeginInfo renderPassBeginInfo{};
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.renderPass = appScanlineRenderPass;
            renderPassBeginInfo.framebuffer = appSwapchainFramebuffers[imageIndex];
            renderPassBeginInfo.renderArea.offset = { 0, 0 };
            renderPassBeginInfo.renderArea.extent = appSwapchainExtent;

            // Order match attachments
            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
            clearValues[1].depthStencil = { 1.0f, 0 };

            renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassBeginInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
            {
                vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, appScanlinePipeline);
                vkCmdBindDescriptorSets(commandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    appScanlinePipelineLayout,
                    0,
                    1,
                    &appScanlineDescriptorSets[appCurrentFrame],
                    0,
                    nullptr);

                VkViewport viewport{};
                viewport.x = 0.0f;
                viewport.y = 0.0f;
                viewport.width = (float)appSwapchainExtent.width;
                viewport.height = (float)appSwapchainExtent.height;
                viewport.minDepth = 0.0f;
                viewport.maxDepth = 1.0f;
                vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

                VkRect2D scissor{};
                scissor.offset = { 0, 0 };
                scissor.extent = appSwapchainExtent;
                vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

                for (auto object : appObjects)
                {

                    // Push Constant

                    VkDeviceSize offsets[] = { 0 }; // make it cache friendly by bind all vertices together and use offset
                    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &object.vertexBuffer.buffer, offsets);
                    vkCmdBindIndexBuffer(commandBuffer, object.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

                    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(object.nbIndices), 1, 0, 0, 0);
                }

                /*
                VkDeviceSize offsets[] = { 0 }; // make it cache friendly by bind all vertices together and use offset
                vkCmdBindVertexBuffers(commandBuffer, 0, 1, &appVertexBufferWrap.buffer, offsets);
                vkCmdBindIndexBuffer(commandBuffer, appIndexBufferWrap.buffer, 0, VK_INDEX_TYPE_UINT32);

                vkCmdBindDescriptorSets(commandBuffer, 
                                        VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                        appScanlinePipelineLayout, 
                                        0, 
                                        1, 
                                        &appScanlineDescriptorSets[appCurrentFrame], 
                                        0, 
                                        nullptr);

                vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
                */

            }
            vkCmdEndRenderPass(commandBuffer);
        }
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}