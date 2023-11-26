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
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,                            // Memory Property
                1,
                appMsaaSamples);

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
            //std::array<VkImageView, 3> attachments = {
            //    // Write to Scanline ImageWrap
            //    // which will get read by post processing
            //    appMsaaImageWrap.imageView,
            //    appSwapchainDepthImageWrap.imageView,
            //    appScanlineImageWrap[i].imageView
            //};

            std::array<VkImageView, 2> attachments = {
                // Write to Scanline ImageWrap
                // which will get read by post processing
                appScanlineImageWrap[i].imageView,
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

            if (vkCreateFramebuffer(appDevice, &framebufferInfo, nullptr, &appScanlineFrameBuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    void ReinkanApp::CreateShadowFrameBuffers()
    {
        // Render Target
        appShadowMapImageWraps.resize(MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            appShadowMapImageWraps[i] = CreateImageWrap(appShadowMapWidth,
                appShadowMapHeight,
                VK_FORMAT_R32G32B32A32_SFLOAT,                                           // Image Format
                VK_IMAGE_TILING_OPTIMAL,                                        // Image Tilling
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT                             // As a result for render
                | VK_IMAGE_USAGE_TRANSFER_DST_BIT
                | VK_IMAGE_USAGE_SAMPLED_BIT,                                   // Image Usage
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,                            // Memory Property
                1,
                appMsaaSamples);

            TransitionImageLayout(appShadowMapImageWraps[i].image,
                VK_FORMAT_R32G32B32A32_SFLOAT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_GENERAL);

            appShadowMapImageWraps[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;

            appShadowMapImageWraps[i].imageView = CreateImageView(appShadowMapImageWraps[i].image, VK_FORMAT_R32G32B32A32_SFLOAT);
            appShadowMapImageWraps[i].sampler = CreateImageSampler();
        }


        appShadowFrameBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {

            std::array<VkImageView, 2> attachments = {
                // Write to Scanline ImageWrap
                // which will get read by post processing
                appShadowMapImageWraps[i].imageView,
                appSwapchainDepthImageWrap.imageView
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = appShadowRenderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = appShadowMapWidth;
            //framebufferInfo.flags = VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT;
            framebufferInfo.height = appShadowMapHeight;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(appDevice, &framebufferInfo, nullptr, &appShadowFrameBuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    void ReinkanApp::CreateVLightFrameBuffers()
    {
        appVLightingRenderTargetImageWraps.resize(MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            appVLightingRenderTargetImageWraps[i] = CreateImageWrap(appSwapchainExtent.width,
                appSwapchainExtent.height,
                appSwapchainImageFormat,                                        // Image Format
                VK_IMAGE_TILING_OPTIMAL,                                        // Image Tilling
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT                             // As a result for render
                | VK_IMAGE_USAGE_TRANSFER_DST_BIT
                | VK_IMAGE_USAGE_SAMPLED_BIT,                                   // Image Usage
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,                            // Memory Property
                1,
                appMsaaSamples);

            TransitionImageLayout(appVLightingRenderTargetImageWraps[i].image,
                appSwapchainImageFormat,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_GENERAL);

            appVLightingRenderTargetImageWraps[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;

            appVLightingRenderTargetImageWraps[i].imageView = CreateImageView(appVLightingRenderTargetImageWraps[i].image, appSwapchainImageFormat);
            appVLightingRenderTargetImageWraps[i].sampler = CreateImageSampler();
        }

        appVLightFrameBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            std::array<VkImageView, 2> attachments = {
                // Write to appVLightingRenderTargetImageWraps
                appVLightingRenderTargetImageWraps[i].imageView,
                appSwapchainDepthImageWrap.imageView
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = appVLightRenderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = appSwapchainExtent.width;
            framebufferInfo.height = appSwapchainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(appDevice, &framebufferInfo, nullptr, &appVLightFrameBuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }
}