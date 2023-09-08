#include "pch.h"
#include "GraphicsSystem.h"

#include "ResourceLoader/ReinkanModelLoader.h"

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

                //Reinkan::ModelData model;
                ReadAssimpFile("../assets/models/simpleshape.obj",
                    glm::mat4(1.0),
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

                //ModelData sponza;
                ReadAssimpFile("../assets/models/sponza.obj",
                    glm::mat4(0.01),
                    modelDatas,
                    vulkanApp->GetAppMaterialPool(),
                    vulkanApp->GetAppTexturePool(),
                    vulkanApp->GetAppMaterialPool().size());

                glm::mat4 modelTr = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, 0.0));
                //glm::mat4 vampireTr = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, 0.0));
                //glm::mat4 sponzaTr = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, 0.0));
                //vulkanApp->LoadModel(std::make_shared<Reinkan::ModelData>(model), modelTr);
                //vulkanApp->LoadModel(std::make_shared<Reinkan::ModelData>(vampire), vampireTr);
                //vulkanApp->LoadModel(std::make_shared<Reinkan::ModelData>(sponza), sponzaTr);

                for (int i = 0; i < modelDatas.size(); ++i)
                {
                    vulkanApp->LoadModel(modelDatas[i], modelTr);
                }


                // Lights
                vulkanApp->AppendLight({ glm::vec3(2.0, 1.0, 1.0), glm::vec3(1.0,0.0,0.0), 1.0, 1.0 });

                vulkanApp->BindResources();
            }

            ////////////////////////////////////////
            //          Game Object Control
            ////////////////////////////////////////
            vulkanApp->SetEyePosition(0.0, 3.0, 2.0);

            // set binding to action
            // CreateAction 
            // action call camera

	}

	void GraphicsSystem::Update()
	{
        ////////////////////////////////////////
        //          Vulkan Main Program
        ////////////////////////////////////////
        vulkanApp->ReinkanUpdate();
	}
	
	void GraphicsSystem::Destroy()
	{
        vulkanApp->Cleanup();
	}

    std::shared_ptr<ReinkanApp> GraphicsSystem::GetVulkanApp() const
    {
        return vulkanApp;
    }
}

