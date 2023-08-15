#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "Structure/ImageWrap.h"
#include "Structure/BufferWrap.h"

namespace Reinkan
{
    class DescriptorWrap
    {
    public:
        std::vector<VkDescriptorSetLayoutBinding> bindingTable;

        VkDescriptorSetLayout descriptorSetLayout;

        VkDescriptorPool descriptorPool;

        std::vector<VkDescriptorSet> descriptorSets;    // Could be  vector<VkDescriptorSet> for multiple sets;

        void SetBindings(const VkDevice device, std::vector<VkDescriptorSetLayoutBinding> bindings, const uint32_t maxSets);

        void Destroy(VkDevice device);

        // Any data can be written into a descriptor set.  Apparently I need only these few types:
        void Write(VkDevice& device, uint32_t index, const VkBuffer& buffer);

        void Write(VkDevice& device, uint32_t index, const std::vector<BufferWrap>& bufferWraps);

        void Write(VkDevice& device, uint32_t index, const VkDescriptorImageInfo& textureDesc);

        void Write(VkDevice& device, uint32_t index, const std::vector<ImageWrap>& textures);

    };

}