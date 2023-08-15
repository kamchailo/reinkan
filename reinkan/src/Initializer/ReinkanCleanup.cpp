#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
	void ReinkanApp::Cleanup()
	{
		CleanupSwapchain();

		for (auto textureImageWrap : appTextureImageWraps)
			textureImageWrap.Destroy(appDevice);

		appMaterialBufferWrap.Destroy(appDevice);

		appScanlineDescriptorWrap.Destroy(appDevice);

		for (auto object : appObjects)
		{
			object.vertexBufferWrap.Destroy(appDevice);
			object.indexBufferWrap.Destroy(appDevice);
		}

		//appTextureImageWrap.Destroy(appDevice);

		vkDestroyPipeline(appDevice, appScanlinePipeline, nullptr);
		vkDestroyPipelineLayout(appDevice, appScanlinePipelineLayout, nullptr);
		vkDestroyRenderPass(appDevice, appScanlineRenderPass, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
		{
			appScanlineUBO[i].Destroy(appDevice);
		}

		vkDestroyDescriptorPool(appDevice, appScanlineDescriptorPool, nullptr);

		vkDestroyDescriptorSetLayout(appDevice, appScanlineDescriptorSetLayout, nullptr);

		appIndexBufferWrap.Destroy(appDevice);
		appVertexBufferWrap.Destroy(appDevice); 

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
		{
			vkDestroySemaphore(appDevice, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(appDevice, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(appDevice, inFlightFences[i], nullptr);
		}

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