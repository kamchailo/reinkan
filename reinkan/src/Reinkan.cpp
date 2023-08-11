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

			// Descriptors
		CreateScanlineDiscriptorSetLayout();

		CreateScanlinePipeline();

		CreateSwapchainFrameBuffers();

		// Command or Drawing Resources Init

		CreateCommandPool();

		CreateCommandBuffer();

		CreateSyncObjects();

		// VertexBuffer

		CreateVertexBuffer(vertices);

		CreateIndexBuffer(indices);

		// Image Loading

		appTextureImageWrap =  CreateTextureImageWrap("../assets/textures/Vampire_diffuse.png");

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

