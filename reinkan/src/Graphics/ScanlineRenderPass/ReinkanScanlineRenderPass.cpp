#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
{
    void ReinkanApp::CreateScanlineRenderPass()
    {
        std::array< VkAttachmentDescription, 4> attacmentDescriptions;

        // Position
        attacmentDescriptions[0].format = VK_FORMAT_R16G16B16A16_SFLOAT;
        attacmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
        attacmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attacmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attacmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attacmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attacmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attacmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_GENERAL;
        attacmentDescriptions[0].flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;

        // Normal
        attacmentDescriptions[1].format = VK_FORMAT_R16G16B16A16_SFLOAT;
        attacmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
        attacmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attacmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attacmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attacmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attacmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attacmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_GENERAL;
        attacmentDescriptions[1].flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;

        // Albedo
        attacmentDescriptions[2].format = VK_FORMAT_R8G8B8A8_UNORM;
        attacmentDescriptions[2].samples = VK_SAMPLE_COUNT_1_BIT;
        attacmentDescriptions[2].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attacmentDescriptions[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attacmentDescriptions[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attacmentDescriptions[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attacmentDescriptions[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attacmentDescriptions[2].finalLayout = VK_IMAGE_LAYOUT_GENERAL;
        attacmentDescriptions[2].flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;

        // Depth Stencil
        attacmentDescriptions[3].format = FindDepthFormat();
        attacmentDescriptions[3].samples = VK_SAMPLE_COUNT_1_BIT;
        attacmentDescriptions[3].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attacmentDescriptions[3].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attacmentDescriptions[3].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attacmentDescriptions[3].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attacmentDescriptions[3].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attacmentDescriptions[3].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attacmentDescriptions[3].flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;

        std::array< VkAttachmentReference, 3> colorAttachmentReferences;

        // World Position
        colorAttachmentReferences[0].attachment = 0;
        colorAttachmentReferences[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        // Normal
        colorAttachmentReferences[1].attachment = 1;
        colorAttachmentReferences[1].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        // Albedo
        colorAttachmentReferences[2].attachment = 2;
        colorAttachmentReferences[2].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        // Depth Stencil
        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 3;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentReferences.size());
        subpass.pColorAttachments = colorAttachmentReferences.data();
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        // Use subpass dependencies for attachment layout transitions
        std::array<VkSubpassDependency, 2> dependencies;

        dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[0].dstSubpass = 0;
        dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        dependencies[1].srcSubpass = 0;
        dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;


        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attacmentDescriptions.size());
        renderPassInfo.pAttachments = attacmentDescriptions.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
        renderPassInfo.pDependencies = dependencies.data();

        if (vkCreateRenderPass(appDevice, &renderPassInfo, nullptr, &appScanlineRenderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
    }
}
