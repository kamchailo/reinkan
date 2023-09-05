#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
    void ReinkanApp::DestroyComputeClusteredResources()
    {
        appClusteredGridDescriptorWrap.Destroy(appDevice);
        appClusteredCullLightDescriptorWrap.Destroy(appDevice);

        appClusteredPlanes.Destroy(appDevice);
        
        appClusteredGlobalLights.Destroy(appDevice);

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            appClusteredUBO[i].Destroy(appDevice);

            appClusteredGrids[i].Destroy(appDevice);

            vkDestroySemaphore(appDevice, appComputeClusteredFinishedSemaphores[i], nullptr);
            vkDestroyFence(appDevice, appComputeClusteredInFlightFences[i], nullptr);

            if (appLightObjects.size() == 0) { break; }
            
            appClusteredLightIndexMap[i].Destroy(appDevice);

            appClusteredLightGrid[i].Destroy(appDevice);
        
        }
    }
}