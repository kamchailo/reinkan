#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

namespace Reinkan::Graphics
{
    void ReinkanApp::UpdateScanlineUBO(uint32_t currentImage)
    {
        UniformBufferObject ubo{};

        ubo.view = appMainCamera->GetViewMatrix();
        ubo.viewInverse = glm::inverse(ubo.view);
        ubo.proj = appMainCamera->GetPerspectiveMatrix();

        auto time = Core::TimeSystemLocator().GetTime();
        //ubo.model = glm::rotate(glm::mat4(1.0f), static_cast<float>(time->GetElapseTime() * glm::radians(90.0f)) * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
         
        ubo.screenExtent = glm::vec2(appSwapchainExtent.width, appSwapchainExtent.height);

        // CPU to update buffer req: VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        memcpy(appScanlineUBOMapped[currentImage], &ubo, sizeof(ubo));

        // GPU to update buffer on VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        //vkCmdUpdateBuffer(...)
        // as vkCmd...
        // Need command buffer and vkCmdPipelineBarrier to wait until buffer is ready to write
        // 
        // Barrier : VK_ACCESS_SHADER_READ_BIT -> VK_ACCESS_TRANSFER_WRITE_BIT
        // vkCmdUpdateBuffer(...)
        // Barrier : VK_ACCESS_TRANSFER_WRITE_BIT -> VK_ACCESS_SHADER_READ_BIT

    }
}

