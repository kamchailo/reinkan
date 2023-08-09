#pragma once

#include <vector>
#include "../shaders/SharedStruct.h"

namespace Reinkan
{
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    const int MAX_FRAMES_IN_FLIGHT = 2;

    const std::vector<const char*> validationLayers = 
    {
        //"VK_LAYER_LUNARG_api_dumps",
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> reqDeviceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,					// Presentation engine; draws to screen
        //VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,		// Ray tracing extension
        //VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,		// Ray tracing extension
        //VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME	// Required by ray tracing pipeline;
    };

    const std::vector<Vertex> vertices = 
    {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
}