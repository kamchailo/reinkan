#include "pch.h"
#include "Reinkan.h"

#include "../shaders/SharedStruct.h"

namespace Reinkan
{
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
            //std::printf("i: %d  near: %f  far: %f size: %f\n", i, clusterPlanes[i].zNear, clusterPlanes[i].zFar, clusterPlanes[i].zFar - clusterPlanes[i].zNear);
        }

        VkDeviceSize bufferSize = sizeof(ClusterPlane) * sizeZ;
        appClusteredPlanes = CreateStagedBufferWrap(bufferSize, clusterPlanes.data(), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    }

    void ReinkanApp::CreateComputeClusteredGrids(uint32_t sizeX, uint32_t sizeY, uint32_t sizeZ)
    {
        // out FrustumCalculation       in CollisionDectection
        //std::vector<BufferWrap>         appClusteredGrids;
        uint32_t bufferSize = sizeof(ClusterGrid) * sizeX * sizeY * sizeZ;
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            appComputeParticleStorageBufferWraps[i] = CreateBufferWrap(bufferSize,
                                                                       VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                                                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        }
    }

    void ReinkanApp::CreateComputeClusteredGlobalLights()
    {
        // readonly in CollisionDectection  readonly in Scanline
        //BufferWrap                      appClusteredGlobalLights;
        if (appLightObjects.size() > 0)
        {
            appClusteredGlobalLights = CreateStagedBufferWrap(appLightObjects, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        }
    }

    void ReinkanApp::CreateComputeClusteredBufferWraps()
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

        // out CollisionDectection          readonly in Scanline
        //std::vector<BufferWrap>         appClusteredLightIndexMap;
        uint32_t maxBufferSize = appLightObjects.size() * 16 * 9 * 32; // N lights * M Grids

        // out CollisionDectection          readonly in Scanline
        //std::vector<BufferWrap>         appClusteredLightGrid;
        uint32_t bufferSize = 16 * 9 * 32;

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

    }

    void ReinkanApp::CreateComputeClusteredSyncObjects()
    {

    }

    void ReinkanApp::CreateComputeClusteredCommandBuffer()
    {

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