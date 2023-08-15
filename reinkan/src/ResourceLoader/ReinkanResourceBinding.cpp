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
            appTextureImageWraps.push_back(CreateTextureImageWrap(texturePath));
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

        bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                  0,                                                            // binding;
                                  VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                            // descriptorType;
                                  1,                                                            // descriptorCount; 
                                  VK_SHADER_STAGE_VERTEX_BIT });                                // stageFlags;
        if (appTextureImageWraps.size() > 0)
        {
            bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                      1,                                                            // binding;
                                      VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,                            // descriptorType;
                                      1,                                                            // descriptorCount;
                                      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT }); // stageFlags;
        }
        if (appTextureImageWraps.size() > 0)
        {
            bindingTable.emplace_back(VkDescriptorSetLayoutBinding{
                                      2,                                                            // binding;
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
        }
        appScanlineDescriptorWrap.Write(appDevice, 0, appScanlineUBO);
        
        // Material only once
        if (appTextureImageWraps.size() > 0)
        {
            appScanlineDescriptorWrap.Write(appDevice, 1, appMaterialBufferWrap.buffer);
        }

        // Texture only once
        if (appTextureImageWraps.size() > 0)
        {
            appScanlineDescriptorWrap.Write(appDevice, 1, appTextureImageWraps);
        }
    }
}