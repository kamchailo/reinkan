#pragma once

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

#include "Constant/CoreConstant.h"
#include "Structure/InitializationStruct.h"
#include "Structure/BufferWrap.h"
#include "Structure/ImageWrap.h"
#include "Structure/ModelData.h"
#include "Structure/ObjectData.h"
#include "Descriptor/DescriptorWrap.h"

//#include "MemoryBinding/ReinkanVertexBuffer.h"
#include "../shaders/SharedStruct.h"

namespace Reinkan
{
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

            CreateScanlineDescriptorWrap();

            CreateScanlinePipeline(appScanlineDescriptorWrap);

            std::printf("\n=============================== END OF BIND RESOURCES ===============================\n\n");
        }

        void SetupScanlinePipeline()
        {
            // Move Bind Resource code part which belong to pipeline here
        }

        void Run() 
        {
            MainLoop();

            std::printf("\n=============================== END OF MAIN LOOP ===============================\n\n");

            Cleanup();
        }

        bool appFramebufferResized = false;

        // ReinkanModelLoader.cpp
        void LoadModel(std::shared_ptr<ModelData> modelData, glm::mat4 transform);

        // ReinkanCamera.cpp
        void SetEyePosition(float eyeX, float eyeY, float eyeZ);

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
        VkQueue appPresentQueue;
        // Compute Shader
        VkQueue appComputeQueue;

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

        VkSwapchainKHR          appSwapchain;
        VkFormat                appSwapchainImageFormat;
        VkExtent2D              appSwapchainExtent;
        std::vector<VkImage>    appSwapchainImages;
        std::vector<VkImageView> appSwapchainImageViews;

    // ReinkanScanlinePipeline.cpp
        void CreateScanlinePipeline(DescriptorWrap& descriptorWrap);

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

        VkCommandBuffer BeginTempCommandBuffer(); // should be change to dedicated Command Buffer for Buffer Initialzation

        void EndTempCommandBuffer(VkCommandBuffer commandBuffer); // should be change to dedicated Command Buffer for Buffer Initialzation

        VkCommandPool appCommandPool;
        std::vector<VkCommandBuffer> appCommandBuffers;

    // ReinkanSyncObjects.cpp
        void CreateSyncObjects();

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

    // ReinkanDrawFrame.cpp
        void DrawFrame();

        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        uint32_t appCurrentFrame = 0;

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

        // Obsolete to Object system
        // void CreateVertexBuffer(std::vector<Vertex> vertices);

        // void CreateIndexBuffer(std::vector<uint32_t> indices);

        BufferWrap appVertexBufferWrap;
        BufferWrap appIndexBufferWrap;

    // ReinkanScanlineUniformBuffer.cpp
        //void CreateScanlineDiscriptorSetLayout();
        //void CreateScanlineDescriptorPool();
        //void CreateScanlineDescriptorSets();
        //void CreateScanlineUniformBuffer();
        void UpdateScanlineUBO(uint32_t currentImage);

        VkDescriptorSetLayout appScanlineDescriptorSetLayout;
        VkDescriptorPool appScanlineDescriptorPool;
        std::vector<VkDescriptorSet> appScanlineDescriptorSets;
        std::vector<BufferWrap> appScanlineUBO;
        std::vector<void*> appScanlineUBOMapped; // Address to Buffer | HOST_VISIBLE

    
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

        std::vector<std::pair<std::shared_ptr<ModelData>, glm::mat4>> appModelDataToBeLoaded;

        // Expose to user to manipulate the objects' properties
        std::vector<ObjectData>     appObjects;

        std::vector<Material>       appMaterials;
        BufferWrap                  appMaterialBufferWrap;

        std::vector<std::string>    appTexturePaths;
        std::vector<ImageWrap>      appTextureImageWraps;

        DescriptorWrap appScanlineDescriptorWrap;

        // ReinkanMultiSampling.cpp
        VkSampleCountFlagBits GetMaxUsableSampleCount();

        void CreateSwapchainColorResources();

        VkSampleCountFlagBits appMsaaSamples = VK_SAMPLE_COUNT_1_BIT;

        ImageWrap appMsaaImageWrap;

    // ReinkanCamera.cpp
        glm::vec3 appEyePosition;
    };
}