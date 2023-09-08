#include "pch.h"
#include "Reinkan.h"

#include "Input/InputManagerLocator.h"

namespace Reinkan
{
	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<ReinkanApp*>(glfwGetWindowUserPointer(window));
		app->appFramebufferResized = true;
	}
    
    
    void ScrollCallback(GLFWwindow* window, double x, double y)
    {
#ifdef GUI
        if (ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().WantCaptureMouse)
            return;
#endif

        int delta = y;
        //if (delta != 0)
            //app->myCamera.wheel(delta > 0 ? 1 : -1);
    }

    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
#ifdef GUI
        if (ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().WantCaptureMouse)
            return;
#endif

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        //app->myCamera.setMousePosition(x, y);
    }

    void CursorPosCallback(GLFWwindow* window, double x, double y)
    {
#ifdef GUI
        if (ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().WantCaptureMouse)
            return;
#endif

        //if (app->myCamera.lmb || app->myCamera.rmb || app->myCamera.mmb)
            //app->myCamera.mouseMove(x, y);
    }

    void CharCallback(GLFWwindow* window, unsigned int key)
    {
#ifdef GUI
        if (ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().WantCaptureKeyboard)
            return;
#endif
    }

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        const bool pressed = action != GLFW_RELEASE;

        if (pressed && key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, 1);
    }
    
	void ReinkanApp::InitWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		appWindow = glfwCreateWindow(windowWidth, windowHeight, "Reinkan", nullptr, nullptr);
		glfwSetWindowUserPointer(appWindow, this);

		glfwSetFramebufferSizeCallback(appWindow, FramebufferResizeCallback);
		//glfwSetCursorPosCallback(appWindow, &CursorPosCallback);
        //glfwSetMouseButtonCallback(appWindow, &MouseButtonCallback);
        //glfwSetScrollCallback(appWindow, &ScrollCallback);

	}
}