#include "pch.h"
#include "Reinkan.h"



namespace Reinkan
{
    VkVertexInputBindingDescription ReinkanApp::GetBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    std::array<VkVertexInputAttributeDescription, 3> ReinkanApp::GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions;
        
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
        
        return attributeDescriptions;
    }

    void ReinkanApp::CreateVertexBuffer(std::vector<Vertex> vertices)
    {
        appVertexBufferWrap = CreateStagedBufferWrap(vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    }

    void ReinkanApp::CreateIndexBuffer(std::vector<uint16_t> indices)
    {
        appIndexBufferWrap = CreateStagedBufferWrap(indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    }
}