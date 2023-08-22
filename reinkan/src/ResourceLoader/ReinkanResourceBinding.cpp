#include "pch.h"
#include "Reinkan.h"

#include <vector>

namespace Reinkan
{
    void Reinkan::ReinkanApp::BindModelData()
    {
        for (auto modelData : appModelDataToBeLoaded)
        {
            ObjectData object;
            object.nbVertices = modelData.first->vertices.size();
            object.nbIndices = modelData.first->indices.size();

            object.transform = modelData.second;

            object.vertexBufferWrap = CreateStagedBufferWrap(modelData.first->vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            object.indexBufferWrap = CreateStagedBufferWrap(modelData.first->indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

            object.objectId = appObjects.size();
            object.materialId =  modelData.first->materialIndex[0];

            appObjects.push_back(object);
        }

        VkPhysicalDeviceProperties gpuProperty;
        vkGetPhysicalDeviceProperties(appPhysicalDevice, &gpuProperty);
        std::printf("Max Descriptor Set Sampled Images: %d\n", gpuProperty.limits.maxDescriptorSetSampledImages);

    }

    void ReinkanApp::BindMaterials()
    {
        if (appMaterials.size() == 0)
        {
            return;
        }
        appMaterialBufferWrap = CreateStagedBufferWrap(appMaterials, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    }

    void ReinkanApp::BindTextures()
    {
        for (auto texturePath : appTexturePaths)
        {
            auto textureImageWrap = CreateTextureImageWrap(texturePath);
            TransitionImageLayout(textureImageWrap.image, 
                VK_FORMAT_R8G8B8A8_SRGB,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            appTextureImageWraps.push_back(textureImageWrap);
        }
    }

    void ReinkanApp::CreateScanlineDescriptorWrap()
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
                                  bindingIndex++,                                                            // binding;
                                  VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                            // descriptorType;
                                  1,                                                            // descriptorCount; 
                                  VK_SHADER_STAGE_VERTEX_BIT });                                // stageFlags;
        if (appMaterials.size() > 0)
        {
            bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                      bindingIndex++,                                                            // binding;
                                      VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,                            // descriptorType;
                                      1,                                                            // descriptorCount;
                                      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT }); // stageFlags;
        }
        if (appTextureImageWraps.size() > 0)
        {
            bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                      bindingIndex++,                                                            // binding;
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,                    // descriptorType;
                                      static_cast<uint32_t>(appTextureImageWraps.size()),           // descriptorCount; // Has to > 0
                                      VK_SHADER_STAGE_FRAGMENT_BIT });                              // stageFlags;
        }

        appScanlineDescriptorWrap.SetBindings(appDevice,
                                              bindingTable,
                                              MAX_FRAMES_IN_FLIGHT);

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
        appScanlineDescriptorWrap.Write(appDevice, 0, appScanlineUBO);
        
        // Material only once
        if (appTextureImageWraps.size() > 0)
        {
            appScanlineDescriptorWrap.Write(appDevice, 1, appMaterialBufferWrap.buffer, MAX_FRAMES_IN_FLIGHT);
        }

        // Texture only once
        if (appTextureImageWraps.size() > 0)
        {
            appScanlineDescriptorWrap.Write(appDevice, 2, appTextureImageWraps, MAX_FRAMES_IN_FLIGHT);
        }
    }
}