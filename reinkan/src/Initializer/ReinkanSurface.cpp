#include "pch.h"
#include "Reinkan.h"

namespace Reinkan
{
    void ReinkanApp::CreateSurface()
    {
        if (glfwCreateWindowSurface(appInstance, appWindow, nullptr, &appSurface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }
}