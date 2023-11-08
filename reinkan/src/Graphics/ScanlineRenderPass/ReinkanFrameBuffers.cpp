#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
{
    void ReinkanApp::CreateScanlineFrameBuffers()
    {
        appScanlineImageWrap.resize(MAX_FRAMES_IN_FLIGHT);

        VkFormat colorFormat = FindDepthFormat();

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            appScanlineImageWrap[i] = CreateImageWrap(appSwapchainExtent.width,
                appSwapchainExtent.height,
                appSwapchainImageFormat,                                        // Image Format
                VK_IMAGE_TILING_OPTIMAL,                                        // Image Tilling
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT                             // As a result for render
                | VK_IMAGE_USAGE_TRANSFER_DST_BIT
                | VK_IMAGE_USAGE_SAMPLED_BIT,                                   // Image Usage
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);                            // Memory Property

            TransitionImageLayout(appScanlineImageWrap[i].image,
                appSwapchainImageFormat,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_GENERAL);


            appScanlineImageWrap[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;

            appScanlineImageWrap[i].imageView = CreateImageView(appScanlineImageWrap[i].image, appSwapchainImageFormat);
            appScanlineImageWrap[i].sampler = CreateImageSampler();
        }

        appScanlineFrameBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            std::array<VkImageView, 3> attachments = {
                // Write to Scanline ImageWrap
                // which will get read by post processing
                appMsaaImageWrap.imageView,
                appSwapchainDepthImageWrap.imageView,
                appScanlineImageWrap[i].imageView
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = appScanlineRenderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = appSwapchainExtent.width;
            framebufferInfo.height = appSwapchainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(appDevice, &framebufferInfo, nullptr, &appScanlineFrameBuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

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
            framebufferInfo.renderPass = appPostRenderPass;
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