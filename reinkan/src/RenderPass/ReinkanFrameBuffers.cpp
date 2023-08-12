#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
    void ReinkanApp::CreateSwapchainFrameBuffers()
    {
        appSwapchainFramebuffers.resize(appSwapchainImageViews.size());

        for (size_t i = 0; i < appSwapchainImageViews.size(); i++) 
        {
            std::array<VkImageView, 2> attachments = {
                            appSwapchainImageViews[i],
                            appSwapchainDepthImageWrap.imageView
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