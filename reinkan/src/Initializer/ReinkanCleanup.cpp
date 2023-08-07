#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
	void ReinkanApp::Cleanup()
	{
		vkDestroySemaphore(appDevice, imageAvailableSemaphore, nullptr);
		vkDestroySemaphore(appDevice, renderFinishedSemaphore, nullptr);
		vkDestroyFence(appDevice, inFlightFence, nullptr);

		vkDestroyCommandPool(appDevice, appCommandPool, nullptr);

		for (auto framebuffer : appSwapchainFramebuffers) {
			vkDestroyFramebuffer(appDevice, framebuffer, nullptr);
		}

		vkDestroyPipeline(appDevice, appScanlinePipeline, nullptr);
		vkDestroyPipelineLayout(appDevice, appScanlinePipelineLayout, nullptr);
		vkDestroyRenderPass(appDevice, appScanlineRenderPass, nullptr);

		for (auto imageView : appSwapchainImageViews) 
		{
			vkDestroyImageView(appDevice, imageView, nullptr);
		}
		
		vkDestroySwapchainKHR(appDevice, appSwapchain, nullptr);

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