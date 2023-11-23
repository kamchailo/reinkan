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

		CreatePostRenderPass();

		CreateShadowRenderPass();

		// MultiSampling
		CreateSwapchainColorResources();

		CreateSwapchainDepthResource();

		// Shadow
		CreateShadowResources(1024, 1024);

		// From VolumicLighting
		CreateVLightResources(1024, 1024);

		CreateSwapchainFrameBuffers(); // require renderpass and resources
		
		CreateScanlineFrameBuffers();

		CreateShadowFrameBuffers();

		// From ComputeClustered
		//CreateComputeClusteredSyncObjects();

		//CreateComputeClusteredCommandBuffer();

		// From ComputeParticle
		//CreateComputeParticleSyncObjects();

		//CreateComputeParticleCommandBuffer();



		// Resources Binding will happen after this point
	}

	void ReinkanApp::BindResources()
	{

		{
			BindModelData();

			BindMaterials();

			BindTextures();

			// Parallax Occlusion Binding
			//BindPyramidalMap(appPyramidalPaths);

			// Clustered
			CreateComputeClusteredBufferWraps(16, 9, 32, 0.1, 1000.0);

			CreateComputeClusteredDescriptorSetWrap();

			CreateClusteredGridPipeline(appClusteredGridDescriptorWrap);

			CreateClusteredCullLightPipeline(appClusteredCullLightDescriptorWrap);

			// Shadow
			CreateShadowDescriptorSetWrap();

			CreateShadowPipeline(appShadowDescriptorWrap);

			// Scanline
			CreateScanlineDescriptorWrap();

			CreateScanlinePipeline(appScanlineDescriptorWrap);

			// Post Processing
			CreatePostDescriptorSetWrap();

			CreatePostPipeline(appPostDescriptorWrap);

			// Debug
			CreateDebugBufferWraps();

			CreateDebugDescriptorSetWrap();

			CreateDebugPipeline(appDebugDescriptorWrap, 
								VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
								VK_POLYGON_MODE_LINE,
								1.0f);

			// Particle
			//CreateComputeParticleBufferWraps();

			//CreateComputeParticleDescriptorSetWrap();

			//CreateComputeParticlePipeline(appComputeParticleDescriptorWrap);

			//CreateComputeParticleSyncObjects()

			std::printf("\n=============================== END OF BIND RESOURCES ===============================\n\n");
		}
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

