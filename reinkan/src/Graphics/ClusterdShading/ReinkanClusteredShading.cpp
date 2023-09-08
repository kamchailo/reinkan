#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

#include "../shaders/SharedStruct.h"

namespace Reinkan::Graphics
{
    void ReinkanApp::CreateComputeClusteredBufferWraps(uint32_t sizeX, uint32_t sizeY, uint32_t sizeZ, float nearClippingPlane, float farClippingPlane)
    {
        CreateComputeClusteredUBO();

        CreateComputeClusteredPlanes(nearClippingPlane, farClippingPlane, sizeZ);

        CreateComputeClusteredGrids(sizeX, sizeY, sizeZ);

        if (appLightObjects.size() == 0) { return; }

        CreateComputeClusteredGlobalLights();

        CreateComputeClusteredLightBuffers(sizeX, sizeY, sizeZ);
    }

    void ReinkanApp::CreateComputeClusteredUBO()
    {
        //std::vector<BufferWrap>         appClusteredUBO;
        //std::vector<void*>              appClusteredUBOMapped;
        VkDeviceSize uboBufferSize = sizeof(ComputeClusteredUniformBufferObject);

        appClusteredUBO.resize(MAX_FRAMES_IN_FLIGHT);
        appClusteredUBOMapped.resize(MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            appClusteredUBO[i] = CreateBufferWrap(uboBufferSize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            vkMapMemory(appDevice, appClusteredUBO[i].memory, 0, uboBufferSize, 0, &appClusteredUBOMapped[i]);
        }
    }

    void ReinkanApp::CreateComputeClusteredPlanes(float nearClippingPlane, float farClippingPlane, uint32_t sizeZ)
    {
        // appClusteredPlanes
        std::vector<ClusterPlane> clusterPlanes;
        clusterPlanes.resize(sizeZ);

        float currentZFar = nearClippingPlane;
        float currentZNear = nearClippingPlane;
        for (int i = 0; i < sizeZ; ++i)
        {
            // take near from last far
            currentZNear = currentZFar;
            clusterPlanes[i].zNear = currentZNear;

            currentZFar = nearClippingPlane * pow(farClippingPlane / nearClippingPlane, static_cast<float>(i+1) / sizeZ);
            clusterPlanes[i].zFar = currentZFar;
            std::printf("i: %d  near: %f  far: %f size: %f\n", i, clusterPlanes[i].zNear, clusterPlanes[i].zFar, clusterPlanes[i].zFar - clusterPlanes[i].zNear);
        }

        VkDeviceSize bufferSize = sizeof(ClusterPlane) * sizeZ;
        appClusteredPlanes = CreateStagedBufferWrap(bufferSize, clusterPlanes.data(), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    }

    void ReinkanApp::CreateComputeClusteredGrids(uint32_t sizeX, uint32_t sizeY, uint32_t sizeZ)
    {
        // out clusteredGrid       in clusteredCullLight
        //std::vector<BufferWrap>         appClusteredGrids;
        uint32_t bufferSize = sizeof(ClusterGrid) * sizeX * sizeY * sizeZ;

        appClusteredGrids.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            appClusteredGrids[i] = CreateBufferWrap(bufferSize,
                                                    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        }
    }

    void ReinkanApp::CreateComputeClusteredGlobalLights()
    {
        // readonly in clusteredCullLight  readonly in Scanline
        //BufferWrap                      appClusteredGlobalLights;
        appClusteredGlobalLights = CreateStagedBufferWrap(appLightObjects, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    }

    void ReinkanApp::CreateComputeClusteredLightBuffers(uint32_t sizeX, uint32_t sizeY, uint32_t sizeZ)
    {
        // out clusteredCullLight          readonly in Scanline
        //std::vector<BufferWrap>         appClusteredLightIndexMap;
        uint32_t maxBufferSize = appLightObjects.size() * sizeX * sizeY * sizeZ; // N lights * M Grids

        // out clusteredCullLight          readonly in Scanline
        //std::vector<BufferWrap>         appClusteredLightGrid;
        uint32_t bufferSize = sizeX * sizeY * sizeZ;

        appClusteredLightIndexMap.resize(MAX_FRAMES_IN_FLIGHT);
        appClusteredLightGrid.resize(MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            appClusteredLightIndexMap[i] = CreateBufferWrap(maxBufferSize,
                                                            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            appClusteredLightGrid[i] = CreateBufferWrap(bufferSize,
                                                        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        }
    }

    void ReinkanApp::CreateComputeClusteredDescriptorSetWrap()
    {   
        //DescriptorWrap                  appClusteredGridDescriptorWrap;
        
        
        
        
        //DescriptorWrap                  appClusteredCullLightDescriptorWrap;



    }

    void ReinkanApp::CreateComputeClusteredSyncObjects()
    {
        appComputeClusteredInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        appComputeClusteredFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(appDevice, &semaphoreInfo, nullptr, &appComputeClusteredFinishedSemaphores[i]) != VK_SUCCESS
                || vkCreateFence(appDevice, &fenceInfo, nullptr, &appComputeClusteredInFlightFences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create compute synchronization objects for a frame!");
            }
        }
    }

    void ReinkanApp::CreateComputeClusteredCommandBuffer()
    {
        appComputeClusteredCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = appCommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)appComputeClusteredCommandBuffers.size();

        if (vkAllocateCommandBuffers(appDevice, &allocInfo, appComputeClusteredCommandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate compute command buffers!");
        }
    }

    void ReinkanApp::UpdateComputeClusteredUBO(uint32_t currentImage)
    {
        ComputeClusteredUniformBufferObject ubo{};
        // update project and view
        ubo.view = glm::mat4();
        ubo.proj = glm::mat4();
        ubo.projInverse = glm::mat4();

        memcpy(appClusteredUBOMapped[currentImage], &ubo, sizeof(ubo));
    }

}