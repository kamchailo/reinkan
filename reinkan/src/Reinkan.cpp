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

		CreateScanlineRenderPass();

		CreateScanlinePipeline();

		CreateSwapchainFrameBuffers();

		// Command or Drawing Resources Init

		CreateCommandPool();

		CreateCommandBuffer();

		CreateSyncObjects();


		std::printf("\n=============================== END of INIT ===============================\n\n");
	}

	void ReinkanApp::MainLoop()
	{
		while (!glfwWindowShouldClose(appWindow)) 
		{
			glfwPollEvents();

			DrawFrame();
		}
	}
}

