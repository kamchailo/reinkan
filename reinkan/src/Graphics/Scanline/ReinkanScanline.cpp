#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

#include "Core/Locator/AnimationSystemLocator.h"

#include <vector>

namespace Reinkan::Graphics
{
    void ReinkanApp::CreateScanlineBufferWrap()
    {
        // UBO [MAX_FRAMES_IN_FLIGHT]
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        appScanlineUBO.resize(MAX_FRAMES_IN_FLIGHT);
        appScanlineUBOMapped.resize(MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            appScanlineUBO[i] = CreateBufferWrap(bufferSize,
                                                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            vkMapMemory(appDevice, appScanlineUBO[i].memory, 0, bufferSize, 0, &appScanlineUBOMapped[i]);
        }

        // Animation UBO
        bufferSize = sizeof(AnimationUniformBufferObject);
        appAnimationMatricesBuffer.resize(MAX_FRAMES_IN_FLIGHT);
        appAnimationMatricesBufferMapped.resize(MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            appAnimationMatricesBuffer[i] = CreateBufferWrap(bufferSize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            vkMapMemory(appDevice, appAnimationMatricesBuffer[i].memory, 0, bufferSize, 0, &appAnimationMatricesBufferMapped[i]);
        }
        

    }

    void ReinkanApp::CreateScanlineDescriptorSet()
    {
        //typedef struct VkDescriptorSetLayoutBinding {
        //    uint32_t              binding;
        //    VkDescriptorType      descriptorType;
        //    uint32_t              descriptorCount;
        //    VkShaderStageFlags    stageFlags;
        //    const VkSampler* pImmutableSamplers;
        //} VkDescriptorSetLayoutBinding;
        std::vector<VkDescriptorSetLayoutBinding> bindingTable;
        uint32_t bindingIndex = 0;
        bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                  bindingIndex++,                                                   // binding;
                                  VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                                // descriptorType;
                                  1,                                                                // descriptorCount; 
                                  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT });     // stageFlags;

        if (appMaterials.size() > 0)
        {
            bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                      bindingIndex++,                                               // binding;
                                      VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,                            // descriptorType;
                                      1,                                                            // descriptorCount;
                                      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT }); // stageFlags;
        }
        if (appTextureImageWraps.size() > 0)
        {
            bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                      bindingIndex++,                                               // binding;
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,                    // descriptorType;
                                      static_cast<uint32_t>(appTextureImageWraps.size()),           // descriptorCount; // Has to > 0
                                      VK_SHADER_STAGE_FRAGMENT_BIT });                              // stageFlags;
        }
        // For Forward 100+ Light
        if (appLightObjects.size() > 0)
        {
            bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                      bindingIndex++,                                       // binding;
                                      VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,                    // descriptorType;
                                      1,                                                    // descriptorCount; 
                                      VK_SHADER_STAGE_FRAGMENT_BIT });                      // stageFlags;
        }
        // ClusterGridBlock
        bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                       bindingIndex++,                                      // binding;
                                       VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,                   // descriptorType;
                                       1,                                                   // descriptorCount; 
                                       VK_SHADER_STAGE_FRAGMENT_BIT });                    // stageFlags;
        // ClusterPlaneBlock
        bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                       bindingIndex++,                                      // binding;
                                       VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,                   // descriptorType;
                                       1,                                                   // descriptorCount; 
                                       VK_SHADER_STAGE_FRAGMENT_BIT });                      // stageFlags;
        // LightIndexSSBO
        bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                       bindingIndex++,                                      // binding;
                                       VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,                   // descriptorType;
                                       1,                                                   // descriptorCount; 
                                       VK_SHADER_STAGE_FRAGMENT_BIT });                      // stageFlags;

        // LightGridSSBO
        bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                       bindingIndex++,                                      // binding;
                                       VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,                   // descriptorType;
                                       1,                                                   // descriptorCount; 
                                       VK_SHADER_STAGE_FRAGMENT_BIT });                      // stageFlags;
        // AnimationMatrices
        bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                        bindingIndex++,                                     // binding;
                                        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                  // descriptorType;
                                        1,                                                  // descriptorCount; 
                                        VK_SHADER_STAGE_VERTEX_BIT });                      // stageFlags;

        appScanlineDescriptorWrap.SetBindings(appDevice,
            bindingTable,
            MAX_FRAMES_IN_FLIGHT);


        appScanlineDescriptorWrap.Write(appDevice, 0, appScanlineUBO);

        // Material only once
        if (appMaterials.size() > 0)
        {
            appScanlineDescriptorWrap.Write(appDevice, 1, appMaterialBufferWrap.buffer, MAX_FRAMES_IN_FLIGHT);
        }

        // Texture only once
        if (appTextureImageWraps.size() > 0)
        {
            appScanlineDescriptorWrap.Write(appDevice, 2, appTextureImageWraps, MAX_FRAMES_IN_FLIGHT);
        }

        // Light Objects
        if (appLightObjects.size() > 0)
        {
            appScanlineDescriptorWrap.Write(appDevice, 3, appClusteredGlobalLights.buffer, MAX_FRAMES_IN_FLIGHT);
        }

        appScanlineDescriptorWrap.Write(appDevice, 4, appClusteredGrids.buffer, MAX_FRAMES_IN_FLIGHT);

        appScanlineDescriptorWrap.Write(appDevice, 5, appClusteredPlanes.buffer, MAX_FRAMES_IN_FLIGHT);

        std::swap(appClusteredLightIndexMap[0], appClusteredLightIndexMap[1]);
        appScanlineDescriptorWrap.Write(appDevice, 6, appClusteredLightIndexMap);
        std::swap(appClusteredLightIndexMap[0], appClusteredLightIndexMap[1]);

        std::swap(appClusteredLightGrid[0], appClusteredLightGrid[1]);
        appScanlineDescriptorWrap.Write(appDevice, 7, appClusteredLightGrid);
        std::swap(appClusteredLightGrid[0], appClusteredLightGrid[1]);

        appScanlineDescriptorWrap.Write(appDevice, 8, appAnimationMatricesBuffer);
        
    }
}