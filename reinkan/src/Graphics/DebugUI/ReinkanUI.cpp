#include "pch.h"
#include "Graphics/ReinkanVulkan.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

/*
* ImGUi integrated into 3 places
* - InitGUI in ReinkanApp::Init()
* - DrawGUI in ReinkanApp::Update()
* - Submit at end of ReinkanApp::DrawFrame() > RecordScanlineCommand();
* **
* - call ImGui::End() when window resize in DrawFrame()
*/
namespace Reinkan::Graphics
{
#ifdef GUI
    void ReinkanApp::InitGUI()
    {
        //uint32_t subpassID = 0;

        // UI
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.LogFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        ImGui::StyleColorsDark();

        std::vector<VkDescriptorPoolSize> poolSize{
            { VK_DESCRIPTOR_TYPE_SAMPLER, 20 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 20 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 20 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 20 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 20 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 20 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 20 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 20 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 20 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 20 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 20 }};

        VkDescriptorPoolCreateInfo    poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 200;
        poolInfo.poolSizeCount = std::size(poolSize);
        poolInfo.pPoolSizes = poolSize.data();
        vkCreateDescriptorPool(appDevice, &poolInfo, nullptr, &AppImguiDescPool);



        // Setup Platform/Renderer back ends
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = appInstance;
        init_info.PhysicalDevice = appPhysicalDevice;
        init_info.Device = appDevice;
        init_info.QueueFamily = appGraphicQueueIndex;
        init_info.Queue = appGraphicsQueue;
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = AppImguiDescPool;
        //init_info.Subpass = subpassID;
        init_info.MinImageCount = 2;
        init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
        init_info.MSAASamples = appMsaaSamples;
        init_info.CheckVkResultFn = nullptr;
        init_info.Allocator = nullptr;

        ImGui_ImplGlfw_InitForVulkan(appWindow, true);


        ImGui_ImplVulkan_Init(&init_info, appScanlineRenderPass);

        // Upload Fonts
        VkCommandBuffer cmdbuf = BeginTempCommandBuffer();
        ImGui_ImplVulkan_CreateFontsTexture(cmdbuf);
        EndTempCommandBuffer(cmdbuf);

    }

    void ReinkanApp::DrawGUI()
    {
        IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        //ImGui::SetNextWindowSize(ImVec2(300, 300));
        ImGui::Begin("Reinkan", NULL);
        
        ImGui::SetWindowFontScale(2.0);

        ImGui::Text("Delta Time: %f", Core::TimeSystemLocator::GetTime()->GetDeltaTime());
        
        ImGui::Text("FPS: %d", Core::TimeSystemLocator::GetTime()->GetStableFPS());

        ImGui::End();
    }

    void ReinkanApp::DestroyGUI()
    {   
        vkDeviceWaitIdle(appDevice);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        vkDestroyDescriptorPool(appDevice, AppImguiDescPool, nullptr);
    }
#endif
}