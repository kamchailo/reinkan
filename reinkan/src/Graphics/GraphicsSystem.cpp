#include "pch.h"
#include "GraphicsSystem.h"

#include "ResourceLoader/ReinkanModelLoader.h"
#include "Core/Locator/AnimationSystemLocator.h"

#include <glm/gtx/transform.hpp>

namespace Reinkan::Graphics
{
    void GraphicsSystem::Init(uint32_t width, uint32_t height)
    {
        ////////////////////////////////////////
        //          Vulkan Init
        ////////////////////////////////////////
        vulkanApp = std::make_shared<ReinkanApp>(width, height);

        vulkanApp->Init();

        ////////////////////////////////////////
        //          Load & Bind Resources
        ////////////////////////////////////////
        {   // Indent to free model data after load

            std::vector<ModelData> modelDatas;

            //ModelData plane;
            glm::mat4 planeTr = glm::scale(glm::mat4(1), glm::vec3(8.0,1.0,8.0));
            ReadAssimpFile("../assets/models/plane.obj",
                planeTr,
                modelDatas,
                vulkanApp->GetAppMaterialPool(),
                vulkanApp->GetAppTexturePool(),
                vulkanApp->GetAppMaterialPool().size());
            /*
            //ModelData SimpleShape;
            glm::mat4 simpleShapeTr = glm::scale(glm::mat4(1), glm::vec3(0.0));
            ReadAssimpFile("../assets/models/simpleshape.obj",
                simpleShapeTr,
                modelDatas,
                vulkanApp->GetAppMaterialPool(),
                vulkanApp->GetAppTexturePool(),
                vulkanApp->GetAppMaterialPool().size());
            

            //ModelData vampire;
            ReadAssimpFile("../assets/models/dancing_vampire.dae",
                glm::mat4(1.0),
                modelDatas,
                vulkanApp->GetAppMaterialPool(),
                vulkanApp->GetAppTexturePool(),
                vulkanApp->GetAppMaterialPool().size());
            */
             
            //ModelData walking;
            glm::mat4 modelTr = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, 0.0));
            ReadAssimpFile("../assets/models/Walking.dae",
                modelTr,
                modelDatas,
                vulkanApp->GetAppMaterialPool(),
                vulkanApp->GetAppTexturePool(),
                vulkanApp->GetAppMaterialPool().size());

            for (int i = 0; i < modelDatas.size(); ++i)
            {
                vulkanApp->LoadModel(modelDatas[i], modelDatas[i].transform);
            }

            vulkanApp->AppendLight({ glm::vec3(0.0, 2.0, 2.0), glm::vec3(1.0,0.0,0.0), 1.0, 1.0 });
        }

        Core::AnimationSystemLocator::GetAnimationSystem()->Init();
        
        vulkanApp->BindResources();

	}

	void GraphicsSystem::Update()
	{
        ////////////////////////////////////////
        //          Vulkan Main Program
        ////////////////////////////////////////
        vulkanApp->UpdateCamera();

        vulkanApp->ReinkanUpdate();
	}
	
	void GraphicsSystem::Destroy()
	{
        vulkanApp->Cleanup();
	}

    ReinkanApp* GraphicsSystem::GetVulkanApp() const
    {
        return vulkanApp.get();
    }
}

