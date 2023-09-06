#include "pch.h"
#include "Reinkan.h"

#include "ParticleSystem/ParticleSystemConstant.h"

namespace Reinkan
{
    void ReinkanApp::RecordComputeCommandBuffer(VkCommandBuffer commandBuffer)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording compute command buffer!");
        }
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, appComputeParticlePipeline);

            vkCmdBindDescriptorSets(commandBuffer,
                VK_PIPELINE_BIND_POINT_COMPUTE,
                appComputeParticlePipelineLayout,
                0,
                1,
                &appComputeParticleDescriptorWrap.descriptorSets[appCurrentFrame],
                0,
                nullptr);

            vkCmdDispatch(commandBuffer, PARTICLE_COUNT / 256, 1, 1);
        }
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record compute command buffer!");
        }
    }
}