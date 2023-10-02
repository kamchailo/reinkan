#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
{
    VkVertexInputBindingDescription ReinkanApp::GetBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    std::array<VkVertexInputAttributeDescription, 13> ReinkanApp::GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 13> attributeDescriptions;
        
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, vertexNormal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, vertexTangent);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, vertexBitangent);

        attributeDescriptions[4].binding = 0;
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(Vertex, texCoord);

        attributeDescriptions[5].binding = 0;
        attributeDescriptions[5].location = 5;
        attributeDescriptions[5].format = VK_FORMAT_R32_SINT;
        attributeDescriptions[5].offset = offsetof(Vertex, boneIds);

        attributeDescriptions[6].binding = 0;
        attributeDescriptions[6].location = 6;
        attributeDescriptions[6].format = VK_FORMAT_R32_SINT;
        attributeDescriptions[6].offset = offsetof(Vertex, boneIds) + sizeof(uint32_t);

        attributeDescriptions[7].binding = 0;
        attributeDescriptions[7].location = 7;
        attributeDescriptions[7].format = VK_FORMAT_R32_SINT;
        attributeDescriptions[7].offset = offsetof(Vertex, boneIds) + 2 * sizeof(uint32_t);

        attributeDescriptions[8].binding = 0;
        attributeDescriptions[8].location = 8;
        attributeDescriptions[8].format = VK_FORMAT_R32_SINT;
        attributeDescriptions[8].offset = offsetof(Vertex, boneIds) + 3 * sizeof(uint32_t);

        attributeDescriptions[9].binding = 0;
        attributeDescriptions[9].location = 9;
        attributeDescriptions[9].format = VK_FORMAT_R32_SFLOAT;
        attributeDescriptions[9].offset = offsetof(Vertex, boneWeights);

        attributeDescriptions[10].binding = 0;
        attributeDescriptions[10].location = 10;
        attributeDescriptions[10].format = VK_FORMAT_R32_SFLOAT;
        attributeDescriptions[10].offset = offsetof(Vertex, boneWeights) + sizeof(uint32_t);

        attributeDescriptions[11].binding = 0;
        attributeDescriptions[11].location = 11;
        attributeDescriptions[11].format = VK_FORMAT_R32_SFLOAT;
        attributeDescriptions[11].offset = offsetof(Vertex, boneWeights) + 2 * sizeof(uint32_t);

        attributeDescriptions[12].binding = 0;
        attributeDescriptions[12].location = 12;
        attributeDescriptions[12].format = VK_FORMAT_R32_SFLOAT;
        attributeDescriptions[12].offset = offsetof(Vertex, boneWeights) + 3 * sizeof(uint32_t);
        
        return attributeDescriptions;
    }

    /*
    void ReinkanApp::CreateVertexBuffer(std::vector<Vertex> vertices)
    {
        appVertexBufferWrap = CreateStagedBufferWrap(vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    }

    void ReinkanApp::CreateIndexBuffer(std::vector<uint32_t> indices)
    {
        appIndexBufferWrap = CreateStagedBufferWrap(indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    }
    */
}