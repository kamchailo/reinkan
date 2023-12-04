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
        appScanlinePositionImageWrap.resize(MAX_FRAMES_IN_FLIGHT);
        appScanlineNormalImageWrap.resize(MAX_FRAMES_IN_FLIGHT);
        appScanlineAlbedoImageWrap.resize(MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            /// Position
            appScanlinePositionImageWrap[i] = CreateImageWrap(appSwapchainExtent.width,
                                                              appSwapchainExtent.height,
                                                              VK_FORMAT_R16G16B16A16_SFLOAT,                                  // Image Format
                                                              VK_IMAGE_TILING_OPTIMAL,                                        // Image Tilling
                                                              VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT                             // As a result for render
                                                              | VK_IMAGE_USAGE_TRANSFER_DST_BIT
                                                              | VK_IMAGE_USAGE_SAMPLED_BIT,                                   // Image Usage
                                                              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,                            // Memory Property
                                                              1,
                                                              appMsaaSamples);

            TransitionImageLayout(appScanlinePositionImageWrap[i].image,
                                  VK_FORMAT_R16G16B16A16_SFLOAT,
                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                  VK_IMAGE_LAYOUT_GENERAL);

            appScanlinePositionImageWrap[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
            appScanlinePositionImageWrap[i].imageView = CreateImageView(appScanlinePositionImageWrap[i].image, VK_FORMAT_R16G16B16A16_SFLOAT);

            /// Normal
            appScanlineNormalImageWrap[i] = CreateImageWrap(appSwapchainExtent.width,
                                                            appSwapchainExtent.height,
                                                            VK_FORMAT_R16G16B16A16_SFLOAT,                                  // Image Format
                                                            VK_IMAGE_TILING_OPTIMAL,                                        // Image Tilling
                                                            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT                             // As a result for render
                                                            | VK_IMAGE_USAGE_TRANSFER_DST_BIT
                                                            | VK_IMAGE_USAGE_SAMPLED_BIT,                                   // Image Usage
                                                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,                            // Memory Property
                                                            1,
                                                            appMsaaSamples);

            TransitionImageLayout(appScanlineNormalImageWrap[i].image,
                                  VK_FORMAT_R16G16B16A16_SFLOAT,
                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                  VK_IMAGE_LAYOUT_GENERAL);

            appScanlineNormalImageWrap[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
            appScanlineNormalImageWrap[i].imageView = CreateImageView(appScanlineNormalImageWrap[i].image, VK_FORMAT_R16G16B16A16_SFLOAT);

            /// Albedo
            appScanlineAlbedoImageWrap[i] = CreateImageWrap(appSwapchainExtent.width,
                                                            appSwapchainExtent.height,
                                                            VK_FORMAT_R8G8B8A8_UNORM,                                        // Image Format
                                                            VK_IMAGE_TILING_OPTIMAL,                                        // Image Tilling
                                                            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT                             // As a result for render
                                                            | VK_IMAGE_USAGE_TRANSFER_DST_BIT
                                                            | VK_IMAGE_USAGE_SAMPLED_BIT,                                   // Image Usage
                                                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,                            // Memory Property
                                                            1,
                                                            appMsaaSamples);

            TransitionImageLayout(appScanlineAlbedoImageWrap[i].image,
                                  VK_FORMAT_R8G8B8A8_UNORM,
                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                  VK_IMAGE_LAYOUT_GENERAL);

            appScanlineAlbedoImageWrap[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
            appScanlineAlbedoImageWrap[i].imageView = CreateImageView(appScanlineAlbedoImageWrap[i].image, VK_FORMAT_R8G8B8A8_UNORM);

            VkSampler colorSampler = CreateImageSampler();
            VkSamplerCreateInfo samplerInfo{};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_NEAREST;
            samplerInfo.minFilter = VK_FILTER_NEAREST;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            samplerInfo.mipLodBias = 0.0f;
            samplerInfo.maxAnisotropy = 1.0f;
            samplerInfo.minLod = 0.0f;
            samplerInfo.maxLod = 1.0f;
            samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
            vkCreateSampler(appDevice, &samplerInfo, nullptr, &colorSampler);

            appScanlinePositionImageWrap[i].sampler = colorSampler;
            appScanlineNormalImageWrap[i].sampler = colorSampler;
            appScanlineAlbedoImageWrap[i].sampler = colorSampler;
        }

        appScanlineFrameBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            std::array<VkImageView, 4> attachments = {
                appScanlinePositionImageWrap[i].imageView,
                appScanlineNormalImageWrap[i].imageView,
                appScanlineAlbedoImageWrap[i].imageView,
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

    void ReinkanApp::CreateScanlineDepthMap()
    {
        appScanlineDepthMapImageWraps.resize(MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            appScanlineDepthMapImageWraps[i] = CreateImageWrap(appSwapchainExtent.width,
                appSwapchainExtent.height,
                VK_FORMAT_R32G32B32_SFLOAT,                                        // Image Format
                VK_IMAGE_TILING_OPTIMAL,                                        // Image Tilling
                VK_IMAGE_USAGE_STORAGE_BIT                             // As a result for render
                | VK_IMAGE_USAGE_TRANSFER_DST_BIT
                | VK_IMAGE_USAGE_SAMPLED_BIT,                                   // Image Usage
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,                            // Memory Property
                1,
                appMsaaSamples);

            TransitionImageLayout(appScanlineDepthMapImageWraps[i].image,
                VK_FORMAT_R32G32B32_SFLOAT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_GENERAL);

            appScanlineDepthMapImageWraps[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;

            appScanlineDepthMapImageWraps[i].imageView = CreateImageView(appScanlineDepthMapImageWraps[i].image, VK_FORMAT_R32G32B32_SFLOAT);
            appScanlineDepthMapImageWraps[i].sampler = CreateImageSampler();
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
            //appShadowMapImageWraps[i].sampler = CreateImageSampler();
            appShadowMapImageWraps[i].sampler = CreateNearestImageSampler();
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
                VK_FORMAT_R32G32B32A32_SFLOAT,                                        // Image Format
                VK_IMAGE_TILING_OPTIMAL,                                        // Image Tilling
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT                             // As a result for render
                | VK_IMAGE_USAGE_TRANSFER_DST_BIT
                | VK_IMAGE_USAGE_SAMPLED_BIT,                                   // Image Usage
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,                            // Memory Property
                1,
                appMsaaSamples);

            TransitionImageLayout(appVLightingRenderTargetImageWraps[i].image,
                VK_FORMAT_R32G32B32A32_SFLOAT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_GENERAL);

            appVLightingRenderTargetImageWraps[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;

            appVLightingRenderTargetImageWraps[i].imageView = CreateImageView(appVLightingRenderTargetImageWraps[i].image, VK_FORMAT_R32G32B32A32_SFLOAT);
            appVLightingRenderTargetImageWraps[i].sampler = CreateImageSampler();
        }

        appVLightFrameBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            std::array<VkImageView, 1> attachments = {
                // Write to appVLightingRenderTargetImageWraps
                appVLightingRenderTargetImageWraps[i].imageView
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