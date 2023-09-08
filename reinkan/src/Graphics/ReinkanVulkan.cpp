#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

namespace Reinkan::Graphics
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
		
		// From ComputeClustered
		CreateComputeClusteredSyncObjects();

		CreateComputeClusteredCommandBuffer();

		// From ComputeParticle
		//CreateComputeParticleSyncObjects();

		//CreateComputeParticleCommandBuffer();

		// Resources Binding will happen after this point
	}

	/* obsolete
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

		std::printf("\n=============================== END OF MAIN LOOP ===============================\n\n");
	}
	*/

	bool ReinkanApp::ShouldClose()
	{
		return glfwWindowShouldClose(appWindow);
	}

	void ReinkanApp::ReinkanUpdate()
	{
		// Calculate Time
		double currentTime = glfwGetTime();
		appLastFrameTime = (currentTime - appLastTime) * 1000.0;
		appLastTime = currentTime;
		double deltaTime = appLastTime - appLastTime;

		glfwPollEvents();

		// Handle Input
		// manual glfw handle input


		// Game Logic


		DrawFrame();

	}
}

