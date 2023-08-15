#include "pch.h"
#include "Reinkan.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace Reinkan
{
    ImageWrap ReinkanApp::CreateTextureImageWrap(std::string path)
	{
        ImageWrap imageWrap;

        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if (!pixels) {
            throw std::runtime_error("failed to load texture image!");
        }

        BufferWrap stagingBufferWrap = CreateBufferWrap(imageSize,
                                                        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        void* data;
        vkMapMemory(appDevice, stagingBufferWrap.memory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(appDevice, stagingBufferWrap.memory);

        stbi_image_free(pixels);

        imageWrap = CreateImageWrap(texWidth,
            texHeight,
            VK_FORMAT_R8G8B8A8_SRGB,                                         // Image Format
            VK_IMAGE_TILING_OPTIMAL,                                         // Image Tilling
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,    // Image Usage
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);                            // Memory Property

        TransitionImageLayout(imageWrap.image,
            VK_FORMAT_R8G8B8A8_SRGB,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        CopyBufferToImage(stagingBufferWrap.buffer,
            imageWrap.image,
            static_cast<uint32_t>(texWidth),
            static_cast<uint32_t>(texHeight));

        TransitionImageLayout(imageWrap.image,
                              VK_FORMAT_R8G8B8A8_SRGB, 
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        stagingBufferWrap.Destroy(appDevice);

        imageWrap.imageView = CreateImageView(imageWrap.image, VK_FORMAT_R8G8B8A8_SRGB);
        imageWrap.sampler = CreateImageSampler();

        return imageWrap;
	}
}
