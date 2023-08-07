#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<ReinkanApp*>(glfwGetWindowUserPointer(window));
		app->appFramebufferResized = true;
	}

	void ReinkanApp::InitWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		appWindow = glfwCreateWindow(windowWidth, windowHeight, "Reinkan", nullptr, nullptr);
		glfwSetWindowUserPointer(appWindow, this);
		glfwSetFramebufferSizeCallback(appWindow, FramebufferResizeCallback);
	}
}