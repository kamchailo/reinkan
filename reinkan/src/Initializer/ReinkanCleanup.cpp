#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
	void ReinkanApp::Cleanup()
	{
		vkDeviceWaitIdle(appDevice);

		CleanupSwapchain();

		vkDestroyPipeline(appDevice, appScanlinePipeline, nullptr);
		vkDestroyPipelineLayout(appDevice, appScanlinePipelineLayout, nullptr);

		vkDestroyRenderPass(appDevice, appScanlineRenderPass, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
		{
			vkDestroySemaphore(appDevice, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(appDevice, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(appDevice, inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(appDevice, appCommandPool, nullptr);

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