#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
{
    void ReinkanApp::CreatePyramidalBufferWraps()
    {
        size_t totalImages = appPyramidalHeightMaps.size();
        appPyramidalImageWraps.resize(totalImages);
        for (int i = 0; i < totalImages; ++i)
        {
            // try to pass through the pixel from texture
            appPyramidalImageWraps[i] = CreateImage3DWrap(appPyramidalHeightMaps[i].width,
                                                        appPyramidalHeightMaps[i].height,
                                                        VK_FORMAT_R32_SFLOAT,  // only one channel is needed
                                                        VK_IMAGE_TILING_OPTIMAL,                                         // Image Tilling
                                                        VK_IMAGE_USAGE_TRANSFER_SRC_BIT  // Added for creating mipmaps which is considers a transfer operation
                                                        | VK_IMAGE_USAGE_TRANSFER_DST_BIT                   
                                                        | VK_IMAGE_USAGE_SAMPLED_BIT 
                                                        | VK_IMAGE_USAGE_STORAGE_BIT,                                   // Image Usage
                                                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,                             // Memory Property
                                                        appPyramidalHeightMaps[i].mipLevels);                           // Mip Levels
            
            appPyramidalImageWraps[i].imageView = CreateImage3DView(appPyramidalImageWraps[i].image, VK_FORMAT_R32_SFLOAT);
            appPyramidalImageWraps[i].sampler = CreateTextureSampler(appPyramidalHeightMaps[i].mipLevels);
            TransitionImageLayout(appPyramidalImageWraps[i].image,
                                VK_FORMAT_R32_SFLOAT,
                                VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_GENERAL,
                                appPyramidalHeightMaps[i].mipLevels);
            appPyramidalImageWraps[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
        }
        /*
        for (auto texturePath : appTexturePaths)
        {
            auto textureImageWrap = CreateTextureImageWrap(texturePath);
            TransitionImageLayout(textureImageWrap.image,
                VK_FORMAT_R8G8B8A8_SRGB,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            appTextureImageWraps.push_back(textureImageWrap);
        }
        */
        appPyramidalHeightMapsBufferWrap = CreateStagedBufferWrap(appPyramidalHeightMaps, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    }

    void ReinkanApp::CreateComputeGeneratePyramidalDescriptorSetWrap()
    {
        // binding
        // 0 : textures[]
        // 1 : heightMapData[]
        // 2 : PyramidalMap[]
        std::vector<VkDescriptorSetLayoutBinding> bindingTable;
        uint32_t bindingIndex = 0;
        
        // TextureImageWraps
        if (appTextureImageWraps.size() > 0)
        {
            bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                      bindingIndex++,                                               // binding;
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,                    // descriptorType;
                                      static_cast<uint32_t>(appTextureImageWraps.size()),           // descriptorCount; // Has to > 0
                                      VK_SHADER_STAGE_COMPUTE_BIT });                              // stageFlags;
        }

        // PyramidalImageWraps
        if (appPyramidalImageWraps.size() > 0)
        {
            bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                      bindingIndex++,                                               // binding;
                                      VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,                    // descriptorType;
                                      static_cast<uint32_t>(appPyramidalImageWraps.size()),           // descriptorCount; // Has to > 0
                                      VK_SHADER_STAGE_COMPUTE_BIT });                              // stageFlags;
        }

        // appPyramidalHeightMapsBufferWrap
        if (appPyramidalHeightMaps.size() > 0)
        {
            bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                      bindingIndex++,                                               // binding;
                                      VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,                            // descriptorType;
                                      1,                                                            // descriptorCount; // Has to > 0
                                      VK_SHADER_STAGE_COMPUTE_BIT });                              // stageFlags;
        }

        appComputeGeneratePyramidalDescriptorWrap.SetBindings(appDevice,
                                                              bindingTable,
                                                              MAX_FRAMES_IN_FLIGHT);


        // Texture only once
        if (appTextureImageWraps.size() > 0)
        {
            appComputeGeneratePyramidalDescriptorWrap.Write(appDevice, 0, appTextureImageWraps, MAX_FRAMES_IN_FLIGHT);
        }

        // PyramidalImageWraps
        if (appPyramidalImageWraps.size() > 0)
        {
            appComputeGeneratePyramidalDescriptorWrap.Write(appDevice, 1, appPyramidalImageWraps, MAX_FRAMES_IN_FLIGHT);
        }

        // PyramidalHeightMaps
        appComputeGeneratePyramidalDescriptorWrap.Write(appDevice, 2, appPyramidalHeightMapsBufferWrap.buffer, MAX_FRAMES_IN_FLIGHT);

    }

    void ReinkanApp::GeneratePyramidalMap()
    {
        vkWaitForFences(appDevice, 1, &appComputeClusteredInFlightFences[appCurrentFrame], VK_TRUE, UINT64_MAX);

        vkResetFences(appDevice, 1, &appComputeClusteredInFlightFences[appCurrentFrame]);

        vkResetCommandBuffer(appComputeClusteredCommandBuffers[appCurrentFrame], 0);

        RecordComputeCommandBuffer(appComputeClusteredCommandBuffers[appCurrentFrame],
                                    appComputeGeneratePyramidalPipeline,
                                    appComputeGeneratePyramidalPipelineLayout,
                                    appComputeGeneratePyramidalDescriptorWrap,
                                    appPyramidalHeightMaps.size(), // Dispatch to the number of pyramidalMaps
                                    1,
                                    1,
                                    VK_TRUE);

        VkSubmitInfo submitComputeInfo{};
        submitComputeInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitComputeInfo.commandBufferCount = 1;
        submitComputeInfo.pCommandBuffers = &appComputeClusteredCommandBuffers[appCurrentFrame];
        //submitComputeInfo.waitSemaphoreCount = 0;
        //submitComputeInfo.pWaitSemaphores = {};
        //submitComputeInfo.signalSemaphoreCount = 1;
        //uint32_t nextFrame = (appCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        //submitComputeInfo.pSignalSemaphores = &appComputeClusteredFinishedSemaphores[appCurrentFrame];

        if (vkQueueSubmit(appComputeQueue, 1, &submitComputeInfo, appComputeClusteredInFlightFences[appCurrentFrame]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit compute command buffer!");
        };
    }
}