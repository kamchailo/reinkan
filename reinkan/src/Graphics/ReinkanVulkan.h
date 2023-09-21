#pragma once

#define GUI

#ifdef GUI
#include "backends/imgui_impl_glfw.h"
#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#endif

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

#include "Graphics/Constant/CoreConstant.h"
#include "Graphics/Structure/InitializationStruct.h"
#include "Graphics/Structure/BufferWrap.h"
#include "Graphics/Structure/ImageWrap.h"
#include "Graphics/Structure/ModelData.h"
#include "Graphics/Structure/ObjectData.h"
#include "Graphics/Descriptor/DescriptorWrap.h"
#include "Camera/Camera.h"

#include "../shaders/SharedStruct.h"

namespace Reinkan::Graphics
{
    typedef void (*updateFunction)();

	class ReinkanApp
	{
    public:
        ReinkanApp(unsigned int width, unsigned int height) 
            : windowWidth(width), windowHeight(height) 
        {}

        void Init()
        {
            InitWindow();

            InitVulkan();

            #ifdef GUI
            InitGUI();
            #endif

            std::printf("\n=============================== END OF INIT ===============================\n\n");
        }
        
        // ------------------------------ TEMP ------------------------------

        std::vector<Material>& GetAppMaterialPool() 
        {
            return appMaterials;
        }

        std::vector<std::string>& GetAppTexturePool() 
        {
            return appTexturePaths;
        }

        void BindResources()
        {
            BindModelData();

            BindMaterials();

            BindTextures();

            // Clustered
            CreateComputeClusteredBufferWraps(16, 9, 32, 0.1, 1000.0);

            CreateComputeClusteredDescriptorSetWrap();

            CreateClusteredGridPipeline(appClusteredGridDescriptorWrap);

            CreateClusteredCullLightPipeline(appClusteredCullLightDescriptorWrap);

            // Scanline
            CreateScanlineDescriptorWrap();

            CreateScanlinePipeline(appScanlineDescriptorWrap);

            //CreateDebugBufferWraps();

            //CreateDebugDescriptorSetWrap();

            //CreateDebugPipeline(appDebugDescriptorWrap, VK_PRIMITIVE_TOPOLOGY_LINE_STRIP);

            // Particle
            //CreateComputeParticleBufferWraps();

            //CreateComputeParticleDescriptorSetWrap();

            //CreateComputeParticlePipeline(appComputeParticleDescriptorWrap);

            //CreateComputeParticleSyncObjects()

            std::printf("\n=============================== END OF BIND RESOURCES ===============================\n\n");
        }

        void SetupScanlinePipeline(std::string vertexShaderPath, std::string fragmentShaderPath, DescriptorWrap& scanlineDescriptorWrap)
        {
            // Move Bind Resource code part which belong to pipeline here
        }

    // Reinkan.cpp
        //void MainLoop();

        bool ShouldClose();

        void ReinkanUpdate();

    // ReinkanCleanup.cpp
        void Cleanup();

        bool appFramebufferResized = false;

    // ReinkanModelLoader.cpp
        void LoadModel(const ModelData& modelData, glm::mat4 transform);

    // ReinkanCamera.cpp
        void SetMainCamera(Camera::Camera* camera);

        void UpdateCamera();

        Camera::Camera* GetMainCamera() const;

    // ReinkanLightUtility.cpp
        void AppendLight(const LightObject& lightObject);

    private:
    // Reinkan.cpp
        void InitVulkan();

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

        VkDevice    appDevice;
        VkQueue     appGraphicsQueue;
        VkQueue     appPresentQueue;

        uint32_t    appGraphicQueueIndex;

    // ReinkanSurface.cpp
        void CreateSurface();
        VkSurfaceKHR appSurface;

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

        VkSwapchainKHR              appSwapchain;
        VkFormat                    appSwapchainImageFormat;
        VkExtent2D                  appSwapchainExtent;
        std::vector<VkImage>        appSwapchainImages;
        std::vector<VkImageView>    appSwapchainImageViews;

    // ReinkanMultiSampling.cpp
        VkSampleCountFlagBits GetMaxUsableSampleCount();

        void CreateSwapchainColorResources();

        VkSampleCountFlagBits       appMsaaSamples = VK_SAMPLE_COUNT_1_BIT;

        ImageWrap                   appMsaaImageWrap;

    // ReinkanScanlinePipeline.cpp
        void CreateScanlinePipeline(DescriptorWrap& descriptorWrap);

        VkPipelineLayout            appScanlinePipelineLayout;
        VkPipeline                  appScanlinePipeline;

    // ReinkanPipelineUtility.cpp
        static std::vector<char> ReadFile(const std::string& filename);

        VkShaderModule CreateShaderModule(const std::vector<char>& code);

    // ReinkanRenderPass.cpp
        void CreateScanlineRenderPass();

        VkRenderPass                appScanlineRenderPass;

    // ReinkanFrameBuffer.cpp
        void CreateSwapchainFrameBuffers();

        std::vector<VkFramebuffer>  appSwapchainFramebuffers;

    // ReinkanCommandBuffer.cpp
        void CreateCommandPool();

        void CreateCommandBuffer();

        VkCommandBuffer BeginTempCommandBuffer(); // should be change to dedicated Command Buffer for Buffer Initialization

        void EndTempCommandBuffer(VkCommandBuffer commandBuffer); // should be change to dedicated Command Buffer for Buffer Initialization

        VkCommandPool                appCommandPool;
        std::vector<VkCommandBuffer> appCommandBuffers;

    // ReinkanSyncObjects.cpp
        void CreateSyncObjects();

        std::vector<VkSemaphore>    imageAvailableSemaphores;
        std::vector<VkSemaphore>    renderFinishedSemaphores;
        std::vector<VkFence>        inFlightFences;

    // ReinkanDrawFrame.cpp
        void DrawFrame();

        uint32_t appCurrentFrame = 0;

    // ReinkanRecordScanline.cpp
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    // ReinkanRecordCompute.cpp
        void RecordComputeCommandBuffer(VkCommandBuffer commandBuffer, 
                                        VkPipeline pipeline, 
                                        VkPipelineLayout pipelineLayout, 
                                        DescriptorWrap descriptorWrap, 
                                        uint32_t dispatchCountX, 
                                        uint32_t dispatchCountY, 
                                        uint32_t dispatchCountZ,
                                        bool isMemBarrier);

    ////////////////////////////////////////
    //      Resources Binding
    ////////////////////////////////////////

    // ReinkanBufferUtility.cpp
        BufferWrap CreateBufferWrap(VkDeviceSize size,
                          VkBufferUsageFlags bufferUsage, 
                          VkMemoryPropertyFlags memoryProperties);
        
        BufferWrap CreateStagedBufferWrap(const VkDeviceSize& size,
                                           const void* data, 
                                           VkBufferUsageFlags usage);

        template <typename T>
        BufferWrap CreateStagedBufferWrap(const std::vector<T>& data,
                                          VkBufferUsageFlags     usage)
        {
            return CreateStagedBufferWrap(sizeof(T) * data.size(), data.data(), usage);
        }

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    // ReinkanVertexBuffer.cpp
        static VkVertexInputBindingDescription GetBindingDescription();

        static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions();

    // ReinkanScanlineUniformBuffer.cpp
        void UpdateScanlineUBO(uint32_t currentImage);

        //VkDescriptorSetLayout appScanlineDescriptorSetLayout;
        //VkDescriptorPool appScanlineDescriptorPool;
        std::vector<VkDescriptorSet>    appScanlineDescriptorSets;
        std::vector<BufferWrap>         appScanlineUBO;
        std::vector<void*>              appScanlineUBOMapped; // Address to Buffer | HOST_VISIBLE

    
    // ReinkanImageLoader.cpp
        ImageWrap CreateTextureImageWrap(std::string path);

    // ReinkanImageUtility.cpp
        ImageWrap CreateImageWrap(uint32_t width,
                                  uint32_t height,
                                  VkFormat format,
                                  VkImageTiling tiling,
                                  VkImageUsageFlags usage,
                                  VkMemoryPropertyFlags properties,
                                  uint32_t  mipLevels = 1,
                                  VkSampleCountFlagBits numSamples = VK_SAMPLE_COUNT_1_BIT);

        VkImageView CreateImageView(VkImage image, 
                                    VkFormat format,
                                    VkImageAspectFlagBits aspect = VK_IMAGE_ASPECT_COLOR_BIT,
                                    uint32_t  mipLevels = 1);

        // Can be created as a few unique sampler 
        // instead of create one for each texture, 
        // and let use choose the setting.
        VkSampler CreateImageSampler();

        VkSampler CreateTextureSampler(uint32_t mipLevels);

        void TransitionImageLayout(VkImage image,
                                   VkFormat format,
                                   VkImageLayout oldLayout,
                                   VkImageLayout newLayout,
                                   uint32_t  mipLevels = 1);


        void CopyBufferToImage(VkBuffer buffer, 
                               VkImage image, 
                               uint32_t width, 
                               uint32_t height);

        void CreateMipmaps(VkImage image, 
                           VkFormat imageFormat,
                           int32_t texWidth, 
                           int32_t texHeight, 
                           uint32_t mipLevels);

    // ReinkanDepthBuffer.cpp
        void CreateSwapchainDepthResource();

        VkFormat FindDepthFormat();

        VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates,
                                     VkImageTiling tiling,
                                     VkFormatFeatureFlags features);

        bool HasStencilComponent(VkFormat format);

        ImageWrap appSwapchainDepthImageWrap;

    // ReinkanResourceBinding.cpp
        void BindModelData();

        void BindMaterials();

        void BindTextures();

        void CreateScanlineDescriptorWrap();

        std::vector<ModelDataLoading> appModelDataToBeLoaded;

        std::vector<ObjectData>     appObjects;

        std::vector<Material>       appMaterials;
        BufferWrap                  appMaterialBufferWrap;

        std::vector<std::string>    appTexturePaths;
        std::vector<ImageWrap>      appTextureImageWraps;

        DescriptorWrap              appScanlineDescriptorWrap;

    ////////////////////////////////////////
    //          User Control
    ////////////////////////////////////////

    // ReinkanCamera.cpp
        glm::vec3 appEyePosition;
        std::vector<updateFunction> appUpdates;
        Camera::Camera*             appMainCamera;

    // ReinkanLightUtility.cpp
        std::vector<LightObject>    appLightObjects;

        
    // Expose Object Property from std::vector<ObjectData> appObjects
        // - position
        // - rotation
        // - scale
        // - push constant

    ////////////////////////////////////////
    //          Debug Pass
    ////////////////////////////////////////

    // ReinkanDebugPipeline.cpp
        void CreateDebugPipeline(DescriptorWrap& descriptorWrap, VkPrimitiveTopology primitive);

        VkPipelineLayout            appDebugPipelineLayout;
        VkPipeline                  appDebugPipeline;

    // ReinkanDebug.cpp
        //static VkVertexInputBindingDescription GetDebugBindingDescription();

        //static std::array<VkVertexInputAttributeDescription, 1> GetDebugAttributeDescriptions();

        void CreateDebugBufferWraps();

        void CreateDebugDescriptorSetWrap();

        DescriptorWrap                  appDebugDescriptorWrap;

        std::vector<BufferWrap>         appDebugUBO;
        std::vector<void*>              appDebugUBOMapped;

        std::vector<BufferWrap>         appDebugStorageBufferWraps;

    ////////////////////////////////////////
    //          Debug UI (ImGui)
    ////////////////////////////////////////

    // ReinkanUI.cpp
        #ifdef GUI

        void InitGUI();

        void DrawGUI();

        void DestroyGUI();

        VkDescriptorPool AppImguiDescPool;

        #endif

    ////////////////////////////////////////
    //          Compute Shaders
    ////////////////////////////////////////
        
        VkQueue appComputeQueue;        // Create in CreateDevice();
        uint32_t appComputeQueueIndex;

    // -------- Particle System -------- //

    // ReinkanComputeParticlePipeline.cpp
        void CreateComputeParticlePipeline(DescriptorWrap& descriptorWrap);

        VkPipelineLayout    appComputeParticlePipelineLayout;
        VkPipeline          appComputeParticlePipeline;

    // ReinkanParticleSystem.cpp
        void CreateComputeParticleBufferWraps();

        void CreateComputeParticleDescriptorSetWrap();

        void CreateComputeParticleSyncObjects();

        void CreateComputeParticleCommandBuffer();

        void UpdateComputeParticleUBO(uint32_t currentImage);

        static VkVertexInputBindingDescription GetParticleBindingDescription();

        static std::array<VkVertexInputAttributeDescription, 2> GetParticleAttributeDescriptions();

        DescriptorWrap                  appComputeParticleDescriptorWrap;
        
        std::vector<BufferWrap>         appComputeParticleUBO;
        std::vector<void*>              appComputeParticleUBOMapped;

        std::vector<BufferWrap>         appComputeParticleStorageBufferWraps;

        std::vector<VkFence>            appComputeParticleInFlightFences;
        std::vector<VkSemaphore>        appComputeParticleFinishedSemaphores;

        std::vector<VkCommandBuffer>    appComputeParticleCommandBuffers;
        float                           appLastFrameTime = 0.0f;
        double                          appLastTime = 0.0f;

        void DestroyComputeParticleResources();

    // -------- Clustered Shading -------- //

    // ReinkanClusteredPipeline.cpp
        void CreateClusteredGridPipeline(DescriptorWrap& descriptorWrap);

        VkPipelineLayout    appClusteredGridPipelineLayout;
        VkPipeline          appClusteredGridPipeline;

        void CreateClusteredCullLightPipeline(DescriptorWrap& descriptorWrap);

        VkPipelineLayout    appClusteredCullLightPipelineLayout;
        VkPipeline          appClusteredCullLightPipeline;
    
    // ReinkanClusteredShading.cpp
        void CreateComputeClusteredBufferWraps(uint32_t sizeX, uint32_t sizeY, uint32_t sizeZ, float nearClippingPlane, float farClippingPlane);

        void CreateComputeClusteredUBO();

        void CreateComputeClusteredPlanes(float nearClippingPlane, float farClippingPlane, uint32_t sizeZ);

        void CreateComputeClusteredGrids(uint32_t sizeX, uint32_t sizeY, uint32_t sizeZ);

        void CreateComputeClusteredGlobalLights();

        void CreateComputeClusteredLightBuffers(uint32_t sizeX, uint32_t sizeY, uint32_t sizeZ);

        void CreateComputeClusteredDescriptorSetWrap();

        void CreateComputeClusteredSyncObjects();

        void CreateComputeClusteredCommandBuffer();

        void CreateComputeClusteredGlobalIndexCount();

        void UpdateComputeClusteredUBO(uint32_t currentImage);

        void UpdateClusteredGrids();

        DescriptorWrap                  appClusteredGridDescriptorWrap;
        DescriptorWrap                  appClusteredCullLightDescriptorWrap;

        std::vector<BufferWrap>         appClusteredUBO;
        std::vector<void*>              appClusteredUBOMapped;
        
        // readonly in clusteredGrid
        BufferWrap                      appClusteredPlanes;
        
        // out clusteredGrid                in clusteredCullLight
        BufferWrap                      appClusteredGrids;
        
        // readonly in clusteredCullLight  readonly in Scanline
        BufferWrap                      appClusteredGlobalLights;
        
        // in clusteredCullLight        Temp Buffer
        std::vector<BufferWrap>         appClusteredGlobalIndexCount;

        // out clusteredCullLight          readonly in Scanline
        std::vector<BufferWrap>         appClusteredLightIndexMap;
        
        // out clusteredCullLight          readonly in Scanline
        std::vector<BufferWrap>         appClusteredLightGrid;
        // in clusteredCullLight            out clusteredCullLight 
        BufferWrap                      appClusteredGlobalLights_DEBUG;

        std::vector<VkCommandBuffer>    appComputeClusteredCommandBuffers;
        std::vector<VkFence>            appComputeClusteredInFlightFences;
        std::vector<VkSemaphore>        appComputeClusteredFinishedSemaphores;

        // ClusteredShader Specification
        uint32_t appClusteredSizeX;
        uint32_t appClusteredSizeY;
        uint32_t appClusteredSizeZ;

        // To Up date ClusterGrids
        bool appIsClusteredGridReady = false;

    // ReinkanComputeClusteredCleanup.cpp
        void DestroyComputeClusteredResources();

    };
}