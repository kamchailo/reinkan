#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
{
    void ReinkanApp::CreateSwapchainFrameBuffers()
    {
        appSwapchainFramebuffers.resize(appSwapchainImageViews.size());

        for (size_t i = 0; i < appSwapchainImageViews.size(); i++) 
        {
            std::array<VkImageView, 3> attachments = {
                            // Make MSAA first attachment
                            // to display MSAA instead of 
                            // Swapchain ImageViews
                            appMsaaImageWrap.imageView,
                            appSwapchainDepthImageWrap.imageView,
                            appSwapchainImageViews[i]
                            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = appScanlineRenderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = appSwapchainExtent.width;
            framebufferInfo.height = appSwapchainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(appDevice, &framebufferInfo, nullptr, &appSwapchainFramebuffers[i]) != VK_SUCCESS) 
            {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }
}