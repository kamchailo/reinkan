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

		// Below this point can move to resources binding stage 
		// ---------------------------------------------------
		// May have to split pipeline to take pipelineLayout after DescriptorSet

			// Descriptors
		//CreateScanlineDiscriptorSetLayout();

		//CreateScanlinePipeline();

		// VertexBuffer -> Move to Bind Loaded Resources

		//CreateVertexBuffer(vertices);

		//CreateIndexBuffer(indices);

		// Image Loading

		//appTextureImageWrap =  CreateTextureImageWrap("../assets/textures/Vampire_diffuse.png");

		// UniformBuffer

		//CreateScanlineDescriptorPool();
		
		//CreateScanlineUniformBuffer(); // UBO before DescriptorSet

		//CreateScanlineDescriptorSets();


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

