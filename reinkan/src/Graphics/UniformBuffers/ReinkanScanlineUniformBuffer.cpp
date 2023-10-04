#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Locator/AnimationSystemLocator.h"
    
namespace Reinkan::Graphics
{
    void ReinkanApp::UpdateScanlineUBO(uint32_t currentImage)
    {
        UniformBufferObject ubo{};

        ubo.view = appMainCamera->GetViewMatrix();
        ubo.viewInverse = glm::inverse(ubo.view);
        ubo.proj = appMainCamera->GetPerspectiveMatrix();

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

    void ReinkanApp::UpdateAnimationMatricesUBO(uint32_t currentImage)
    {
        auto animationSystem = Core::AnimationSystemLocator::GetAnimationSystem();
        AnimationUniformBufferObject ubo{};

        ubo = animationSystem->GetcurrentAnimator()->GetFinalBoneMatrices();
        memcpy(appAnimationMatricesBufferMapped[currentImage], &ubo, sizeof(AnimationUniformBufferObject));

        auto vertDebug = animationSystem->GetcurrentAnimator()->GetDebugVertices();
        memcpy(appDebugStorageMapped[currentImage], &vertDebug, sizeof(glm::vec3) * 200);

        /*
        std::vector<std::pair<glm::vec3, glm::vec3>> debugVert = animationSystem->GetcurrentAnimator()->GetDebugVertices();
        memcpy(appDebugStorageMapped[currentImage], &debugVert, sizeof(glm::vec3) * 200);

        std::printf("-------------------------------\n");
        std::printf(" size of animation UBO %d\n", sizeof(AnimationUniformBufferObject) * ubo.size());
        std::printf(" size of animation Vert UBO %d\n", sizeof(glm::vec3) * 2 * debugVert.size());
        std::printf("-------------------------------\n");
        */
    }
}

