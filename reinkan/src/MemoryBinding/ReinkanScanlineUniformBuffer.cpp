#include "pch.h"
#include "Reinkan.h"


#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

namespace Reinkan
{
    void ReinkanApp::UpdateScanlineUBO(uint32_t currentImage)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ubo.view = glm::lookAt(appEyePosition, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ubo.viewInverse = glm::inverse(ubo.view);
        ubo.proj = glm::perspective(glm::radians(45.0f), appSwapchainExtent.width / (float)appSwapchainExtent.height, 0.1f, 100.0f);
        ubo.proj[1][1] *= -1;
        
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

