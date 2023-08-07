#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
    void ReinkanApp::DrawFrame()
    {
        vkWaitForFences(appDevice, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(appDevice, 1, &inFlightFence);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(appDevice, appSwapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
        // After finish on GPU
        // > > > > > > > [SIGNAL] imageAvailableSemaphore
        
        vkResetCommandBuffer(appCommandBuffer, 0);
        RecordCommandBuffer(appCommandBuffer, imageIndex);

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
        VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &appCommandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        // [WAIT] imageAvailableSemaphore
        if (vkQueueSubmit(appGraphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) 
        // After finish on GPU
        // > > > > > > > [SIGNAL] renderFinishedSemaphore
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

        // [WAIT] renderFinishedSemaphore
        vkQueuePresentKHR(appPresentQueue, &presentInfo);

    }
}