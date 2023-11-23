#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
{
    void ReinkanApp::DestroyShadowResources()
    {
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            appShadowUBO[i].Destroy(appDevice);
            appShadowMapImageWraps[i].Destroy(appDevice);
        }

        //appShadowDescriptorWrap.Destroy(appDevice);
    }
}