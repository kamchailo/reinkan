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

		CreateSwapchainDepthResource();

		CreateSwapchainFrameBuffers(); // require renderpass

			// Descriptors
		CreateScanlineDiscriptorSetLayout();

		CreateScanlinePipeline();

		// VertexBuffer

		CreateVertexBuffer(vertices);

		CreateIndexBuffer(indices);

		// Image Loading

		appTextureImageWrap =  CreateTextureImageWrap("../assets/textures/cloth_knit_feb-20-2022_basecolor.png");

		// UniformBuffer

		CreateScanlineDescriptorPool();
		
		CreateScanlineUniformBuffer(); // UBO before DescriptorSet

		CreateScanlineDescriptorSets();


		std::printf("\n=============================== END OF INIT ===============================\n\n");
	}

	void ReinkanApp::MainLoop()
	{
		while (!glfwWindowShouldClose(appWindow)) 
		{
			glfwPollEvents();

			DrawFrame();
		}

		vkDeviceWaitIdle(appDevice);

		std::printf("\n=============================== END OF MAIN LOOP ===============================\n\n");
	}
}

