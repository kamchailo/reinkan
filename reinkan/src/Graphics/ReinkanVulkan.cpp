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

	void ReinkanApp::BindResources()
	{
		BindModelData();

		BindMaterials();

		BindTextures();

		// Clustered
		CreateComputeClusteredBufferWraps(16, 9, 32, 0.1, 1000.0);

		CreateComputeClusteredDescriptorSetWrap();

		CreateClusteredGridPipeline(appClusteredGridDescriptorWrap);

		CreateClusteredCullLightPipeline(appClusteredCullLightDescriptorWrap);

		// Scanline
		CreateScanlineBufferWrap();

		CreateScanlineDescriptorSet();

		CreateScanlinePipeline(appScanlineDescriptorWrap);

		// Debug
		CreateDebugBufferWraps();

		CreateDebugDescriptorSetWrap();

		CreateDebugPipeline(appDebugDescriptorWrap, 
							VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
							VK_POLYGON_MODE_LINE,
							1.0f);

		// Particle
		//CreateComputeParticleBufferWraps();

		//CreateComputeParticleDescriptorSetWrap();

		//CreateComputeParticlePipeline(appComputeParticleDescriptorWrap);

		//CreateComputeParticleSyncObjects()

		std::printf("\n=============================== END OF BIND RESOURCES ===============================\n\n");
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

