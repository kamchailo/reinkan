#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
	void ReinkanApp::Cleanup()
	{
		std::printf("\n=============================== START OF CLEAN UP ===============================\n\n");

		vkDeviceWaitIdle(appDevice);

		CleanupSwapchain();

		{ // ComputeParticle
		// Resources
			appComputeParticleDescriptorWrap.Destroy(appDevice);
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				appComputeParticleUBO[i].Destroy(appDevice);
				appComputeParticleStorageBufferWraps[i].Destroy(appDevice);
			}
		// Pipeline
			vkDestroyPipeline(appDevice, appComputeParticlePipeline, nullptr);
			vkDestroyPipelineLayout(appDevice, appComputeParticlePipelineLayout, nullptr);
		// Sync
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				vkDestroySemaphore(appDevice, appComputeParticleFinishedSemaphores[i], nullptr);
				vkDestroyFence(appDevice, appComputeParticleInFlightFences[i], nullptr);
			}
		} // End ComputeParticle

		{ // Scanline 
		// Resources
			for (auto textureImageWrap : appTextureImageWraps)
				textureImageWrap.Destroy(appDevice);

			appMaterialBufferWrap.Destroy(appDevice);
			appScanlineDescriptorWrap.Destroy(appDevice);

			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
			{
				appScanlineUBO[i].Destroy(appDevice);
			}

			for (auto object : appObjects)
			{
				object.vertexBufferWrap.Destroy(appDevice);
				object.indexBufferWrap.Destroy(appDevice);
			}
		// Pipeline & RenderPass
			vkDestroyPipeline(appDevice, appScanlinePipeline, nullptr);
			vkDestroyPipelineLayout(appDevice, appScanlinePipelineLayout, nullptr);
			vkDestroyRenderPass(appDevice, appScanlineRenderPass, nullptr);
		// Sync
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
			{
				vkDestroySemaphore(appDevice, renderFinishedSemaphores[i], nullptr);
				vkDestroySemaphore(appDevice, imageAvailableSemaphores[i], nullptr);
				vkDestroyFence(appDevice, inFlightFences[i], nullptr);
			}
		} // End Scanline Resources

		vkDestroyCommandPool(appDevice, appCommandPool, nullptr);

		// Destroy Foundation
		vkDestroyDevice(appDevice, nullptr);

		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(appInstance, appDebugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(appInstance, appSurface, nullptr);
		vkDestroyInstance(appInstance, nullptr);

		glfwDestroyWindow(appWindow);

		glfwTerminate();
	}
}