#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
	void ReinkanApp::InitVulkan()
	{
		// Hardware Support Init
		
		CreateInstance();

		SetupDebugMessenger();

		CreateSurface();

		PickPhysicalDevice();

		CreateLogicalDevice();

		// Rendering Environment Init

		CreateSwapchain();

		CreateSwapchainImageViews();

		// Command or Drawing Resources Init

		CreateCommandPool();

		CreateCommandBuffer();

		CreateSyncObjects();

		// Render Pass
		CreateScanlineRenderPass();

		// MultiSampling
		CreateSwapchainColorResources();

		CreateSwapchainDepthResource();

		CreateSwapchainFrameBuffers(); // require renderpass
		
		// From ComputeParticle
		CreateComputeParticleSyncObjects();

		CreateComputeParticleCommandBuffer();

		// Resources Binding will happen after this point
	}

	void ReinkanApp::MainLoop()
	{
		while (!glfwWindowShouldClose(appWindow)) 
		{
			glfwPollEvents();

			DrawFrame();

			double currentTime = glfwGetTime();
			appLastFrameTime = (currentTime - appLastTime) * 1000.0;
			appLastTime = currentTime;
		}

		vkDeviceWaitIdle(appDevice);

	}
}

