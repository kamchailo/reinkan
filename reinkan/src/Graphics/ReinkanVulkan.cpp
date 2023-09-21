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

	bool ReinkanApp::ShouldClose()
	{
		return glfwWindowShouldClose(appWindow);
	}

	void ReinkanApp::ReinkanUpdate()
	{
		glfwPollEvents();

#ifdef GUI
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		DrawGUI();
#endif

		DrawFrame();

	}
}
