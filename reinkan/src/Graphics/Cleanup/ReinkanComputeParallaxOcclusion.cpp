#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
{
    void ReinkanApp::DestroyComputeGeneratePyramidalResources()
    {
        appComputeGeneratePyramidalDescriptorWrap.Destroy(appDevice);

        for (auto& pyramidalImages : appPyramidalImageWraps)
        {
            pyramidalImages.Destroy(appDevice);
        }

        appPyramidalHeightMapsBufferWrap.Destroy(appDevice);

        vkDestroyPipelineLayout(appDevice, appComputeGeneratePyramidalPipelineLayout, nullptr);
        vkDestroyPipeline(appDevice, appComputeGeneratePyramidalPipeline, nullptr);

    }
}