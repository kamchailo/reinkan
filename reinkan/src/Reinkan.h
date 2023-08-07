#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

#include "Constant/CoreConstant.h"
#include "Structure/InitializationStruct.h"

namespace Reinkan
{
	class ReinkanApp
	{
    public:
        ReinkanApp(unsigned int width, unsigned int height) 
            : windowWidth(width), windowHeight(height) 
        {}

        void Run() 
        {
            InitWindow();

            InitVulkan();

            MainLoop();

            Cleanup();
        }

        bool appFramebufferResized = false;

    private:
        // Reinkan.cpp
        void InitVulkan();

        void MainLoop();

        // ReinkanCleanup.cpp
        void Cleanup();

        // ReinkanWindow.cpp
        void InitWindow();

        GLFWwindow* appWindow;
        unsigned int windowWidth;
        unsigned int windowHeight;

        // ReinkanInstance.cpp
        void CreateInstance();

        VkInstance appInstance;

        // ReinkanValidationLayers.cpp
        bool CheckValidationLayerSupport();

        std::vector<const char*> GetRequiredExtensions();

        void SetupDebugMessenger();

        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                              const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                              const VkAllocationCallbacks* pAllocator,
                                              VkDebugUtilsMessengerEXT* pDebugMessenger);

        void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator);
        
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        
        VkDebugUtilsMessengerEXT appDebugMessenger;
        
        // ReinkanPhysicalDevice.cpp
        void PickPhysicalDevice();

        bool IsDeviceSuitable(VkPhysicalDevice device);

        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

        VkPhysicalDevice appPhysicalDevice;

        // ReinkanDevice.cpp
        void CreateLogicalDevice();

        VkDevice appDevice;
        VkQueue appGraphicsQueue;

        // ReinkanSurface.cpp
        void CreateSurface();

        VkSurfaceKHR appSurface;
        VkQueue appPresentQueue;

        // ReinkanSwapchain.cpp
        void CreateSwapchain();

        SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);

        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        VkSurfaceFormatKHR  ChooseSwapchainSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR ChooseSwapchainPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D  ChooseSwapchainExtent(VkSurfaceCapabilitiesKHR& capabilities);

        void CreateSwapchainImageViews();

        void RecreateSwapchain();

        void CleanupSwapchain();

        VkSwapchainKHR          appSwapchain;
        VkFormat                appSwapchainImageFormat;
        VkExtent2D              appSwapchainExtent;
        std::vector<VkImage>    appSwapchainImages;
        std::vector<VkImageView> appSwapchainImageViews;

        // ReinkanScanlinePipeline.cpp
        void CreateScanlinePipeline();

        VkShaderModule CreateShaderModule(const std::vector<char>& code);

        VkPipelineLayout appScanlinePipelineLayout;
        VkPipeline appScanlinePipeline;

        // ReinkanRenderPass.cpp
        void CreateScanlineRenderPass();

        VkRenderPass appScanlineRenderPass;

        // ReinkanFrameBuffer.cpp
        void CreateSwapchainFrameBuffers();

        std::vector<VkFramebuffer> appSwapchainFramebuffers;

        // ReinkanCommandBuffer.cpp
        void CreateCommandPool();

        void CreateCommandBuffer();

        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        VkCommandPool appCommandPool;
        std::vector<VkCommandBuffer> appCommandBuffers;

        // ReinkanSyncObjects.cpp
        void CreateSyncObjects();

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

        // ReinkanDrawFrame.cpp
        void DrawFrame();

        uint32_t appCurrentFrame = 0;

    };
}