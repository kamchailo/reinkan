#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

#include "Graphics/ParticleSystem/ParticleSystemConstant.h"

namespace Reinkan::Graphics
{
    /*
    * To interface this record command
    * - Interface how the pipeline construct
    * - Interface how to let user create descriptor set easily
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
            clearValues[0].color = { {0.05f, 0.05f, 0.07f, 1.0f} };
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
                    &appScanlineDescriptorWrap.descriptorSets[appCurrentFrame],
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
                    // Information pushed at each draw call
                    PushConstantScanline pushConstant{};
                    pushConstant.materialId = object.materialId;
                    pushConstant.objectId = object.objectId;
                    pushConstant.modelMatrix = object.transform;
                    pushConstant.debugFlag = appDebugFlag;

                    vkCmdPushConstants(commandBuffer,
                                       appScanlinePipelineLayout,
                                       VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                                       0,
                                       sizeof(PushConstantScanline),
                                       &pushConstant
                    );

                    VkDeviceSize offsets[] = { 0 }; // make it cache friendly by bind all vertices together and use offset
                    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &object.vertexBufferWrap.buffer, offsets);
                    vkCmdBindIndexBuffer(commandBuffer, object.indexBufferWrap.buffer, 0, VK_INDEX_TYPE_UINT32);

                    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(object.nbIndices), 1, 0, 0, 0);

                }

                // Debug Draw
                {
                    UpdateDebugUBO(appCurrentFrame);

                    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, appDebugPipeline);
                    vkCmdBindDescriptorSets(commandBuffer,
                                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                                            appDebugPipelineLayout,
                                            0,
                                            1,
                                            &appDebugDescriptorWrap.descriptorSets[appCurrentFrame],
                                            0,
                                            nullptr);

                    VkDeviceSize offsets[] = { 0 }; // make it cache friendly by bind all vertices together and use offset
                    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &appClusteredGrids.buffer, offsets);
                    vkCmdDraw(commandBuffer, appClusteredSizeX * appClusteredSizeY * appClusteredSizeZ, 1, 0, 0);
                }
                /*
                */

                #ifdef GUI
                ImGui::Render();  // Rendering UI
                ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
                #endif
            }
            vkCmdEndRenderPass(commandBuffer);
        }
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
} 