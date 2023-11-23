#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
{
	void ReinkanApp::DestroyVLightResources()
	{
		appVLightVertexBufferWrap.Destroy(appDevice);

		appVLightIndexBufferWrap.Destroy(appDevice);

		for (int i = 0; i < appVLightingRenderTargetImageWraps.size(); ++i)
		{
			appVLightingRenderTargetImageWraps[i].Destroy(appDevice);
		}

	}
}