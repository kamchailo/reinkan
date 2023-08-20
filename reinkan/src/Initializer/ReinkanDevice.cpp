#include "pch.h"
#include "Reinkan.h"

#include <set>

namespace Reinkan
{
    void ReinkanApp::CreateLogicalDevice()
    {
        QueueFamilyIndices indices = FindQueueFamilies(appPhysicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        // list of physical device feature to be enabled.
        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        // Multisampling shading
        deviceFeatures.sampleRateShading = VK_TRUE;

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        
        // @@ Enable fratures 11 shaderDrawParameters
        /*
        VkPhysicalDeviceVulkan11Features features11 = {};

        VkPhysicalDeviceFeatures2 physical_features2 = {};
        physical_features2.pNext = &features11;

        vkGetPhysicalDeviceFeatures2(appPhysicalDevice, &physical_features2);

        // Logic if feature is not supported
        if (features11.shaderDrawParameters == VK_FALSE) {
            std::printf("Does not support: shaderDrawParameters");
        }
        */

        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();

        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(reqDeviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = reqDeviceExtensions.data();

        if (enableValidationLayers) 
        {
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else 
        {
            deviceCreateInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(appPhysicalDevice, &deviceCreateInfo, nullptr, &appDevice) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(appDevice, indices.graphicsFamily.value(), 0, &appGraphicsQueue);
        vkGetDeviceQueue(appDevice, indices.presentFamily.value(), 0, &appPresentQueue);
    }
}