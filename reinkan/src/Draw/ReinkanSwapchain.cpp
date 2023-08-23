#include "pch.h"
#include "Reinkan.h"

#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp

namespace Reinkan
{
    void ReinkanApp::CreateSwapchain()
    {
        SwapchainSupportDetails swapChainSupport = QuerySwapchainSupport(appPhysicalDevice);

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapchainSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = ChooseSwapchainPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = ChooseSwapchainExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 
            && imageCount > swapChainSupport.capabilities.maxImageCount) 
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo{};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface = appSurface;

        swapchainCreateInfo.minImageCount = imageCount;
        swapchainCreateInfo.imageFormat = surfaceFormat.format;
        swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainCreateInfo.imageExtent = extent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

        QueueFamilyIndices indices = FindQueueFamilies(appPhysicalDevice);
        uint32_t queueFamilyIndices[] = { indices.graphicsAndComputeFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsAndComputeFamily != indices.presentFamily) 
        {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchainCreateInfo.queueFamilyIndexCount = 2;
            swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        // Almost all GPU has the same queue that support both.
        else 
        {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        swapchainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = presentMode;
        swapchainCreateInfo.clipped = VK_TRUE;

        swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(appDevice, &swapchainCreateInfo, nullptr, &appSwapchain) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create swap chain!");
        }

        // Get VkImage from Swaphcain KHR, Developer does not create VkImage for swapchain manually.
        vkGetSwapchainImagesKHR(appDevice, appSwapchain, &imageCount, nullptr);
        appSwapchainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(appDevice, appSwapchain, &imageCount, appSwapchainImages.data());

        appSwapchainImageFormat = surfaceFormat.format;
        appSwapchainExtent = extent;
    }

    SwapchainSupportDetails ReinkanApp::QuerySwapchainSupport(VkPhysicalDevice device)
    {
        SwapchainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, appSurface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, appSurface, &formatCount, nullptr);
        if (formatCount != 0) 
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, appSurface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, appSurface, &presentModeCount, nullptr);
        if (presentModeCount != 0) 
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, appSurface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR ReinkanApp::ChooseSwapchainSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats) 
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB 
                && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
            {
                return availableFormat;
            }
        }
        return availableFormats[0];
    }

    VkPresentModeKHR ReinkanApp::ChooseSwapchainPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes) 
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D ReinkanApp::ChooseSwapchainExtent(VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }
        else {
            int width, height;
            glfwGetFramebufferSize(appWindow, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void ReinkanApp::CreateSwapchainImageViews()
    {
        appSwapchainImageViews.resize(appSwapchainImages.size());

        for (size_t i = 0; i < appSwapchainImages.size(); i++)
        {
            /*
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = appSwapchainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = appSwapchainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(appDevice, &createInfo, nullptr, &appSwapchainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views!");
            }
            */
            appSwapchainImageViews[i] = CreateImageView(appSwapchainImages[i], appSwapchainImageFormat);
        }
    }

    void ReinkanApp::RecreateSwapchain()
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize(appWindow, &width, &height);

        while (width == 0 || height == 0) 
        {
            glfwGetFramebufferSize(appWindow, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(appDevice);

        CleanupSwapchain();

        CreateSwapchain();
        CreateSwapchainImageViews();
        CreateSwapchainColorResources();
        CreateSwapchainDepthResource();
        CreateSwapchainFrameBuffers();
    }

    void ReinkanApp::CleanupSwapchain()
    {
        for (size_t i = 0; i < appSwapchainFramebuffers.size(); i++) 
        {
            vkDestroyFramebuffer(appDevice, appSwapchainFramebuffers[i], nullptr);
        }

        for (size_t i = 0; i < appSwapchainImageViews.size(); i++) 
        {
            vkDestroyImageView(appDevice, appSwapchainImageViews[i], nullptr);
        }

        appMsaaImageWrap.Destroy(appDevice);

        appSwapchainDepthImageWrap.Destroy(appDevice);

        vkDestroySwapchainKHR(appDevice, appSwapchain, nullptr);
    }
}