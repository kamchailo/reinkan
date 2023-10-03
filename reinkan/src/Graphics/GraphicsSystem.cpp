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

            //Reinkan::ModelData model;
            /*
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

            
            glm::mat4 planeTr = glm::translate(glm::mat4(1), glm::vec3(0.0, 1.0, 0.0));
            //ModelData plane;
            ReadAssimpFile("../assets/models/plane.obj",
                planeTr,
                modelDatas,
                vulkanApp->GetAppMaterialPool(),
                vulkanApp->GetAppTexturePool(),
                vulkanApp->GetAppMaterialPool().size());
            */

            //ModelData sponza;
            ReadAssimpFile("../assets/models/sponza.obj",
                glm::mat4(0.01),
                modelDatas,
                vulkanApp->GetAppMaterialPool(),
                vulkanApp->GetAppTexturePool(),
                vulkanApp->GetAppMaterialPool().size());

             
            glm::mat4 modelTr = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, 0.0));
            //ModelData walking;
            ReadAssimpFile("../assets/models/Walking.dae",
                modelTr,
                modelDatas,
                vulkanApp->GetAppMaterialPool(),
                vulkanApp->GetAppTexturePool(),
                vulkanApp->GetAppMaterialPool().size());

            for (int i = 0; i < modelDatas.size(); ++i)
            {
                vulkanApp->LoadModel(modelDatas[i], glm::mat4(1.5));
            }


            // Lights
            //vulkanApp->AppendLight({ glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0,0.0,0.0), 1.0, 1.0 });

            vulkanApp->AppendLight({ glm::vec3(-1.0, 2.0, 0.0), glm::vec3(1.0,0.0,0.0), 2.0, 1.0 });
            vulkanApp->AppendLight({ glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0,1.0,0.0), 2.0, 1.0 });
            vulkanApp->AppendLight({ glm::vec3(1.0, 2.0, 0.0), glm::vec3(0.0,1.0,0.0), 2.0, 1.0 });

            vulkanApp->AppendLight({ glm::vec3(-1.0, 2.0, 1.0), glm::vec3(1.0,0.0,0.0), 2.0, 1.0 });
            vulkanApp->AppendLight({ glm::vec3(0.0, 2.0, 1.0), glm::vec3(0.0,1.0,0.0), 2.0, 1.0 });
            vulkanApp->AppendLight({ glm::vec3(1.0, 2.0, 1.0), glm::vec3(0.0,1.0,0.0), 2.0, 1.0 });

            vulkanApp->AppendLight({ glm::vec3(-1.0, 1.0, 1.0), glm::vec3(1.0,0.0,0.0), 2.0, 1.0 });
            vulkanApp->AppendLight({ glm::vec3(0.0, 1.0, 1.0), glm::vec3(0.0,1.0,0.0), 2.0, 1.0 });
            vulkanApp->AppendLight({ glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0,1.0,0.0), 2.0, 1.0 });

            vulkanApp->AppendLight({ glm::vec3(-1.0, 2.0, 2.0), glm::vec3(1.0,0.0,0.0), 2.0, 1.0 });
            vulkanApp->AppendLight({ glm::vec3(0.0, 2.0, 2.0), glm::vec3(0.0,1.0,0.0), 2.0, 1.0 });
            vulkanApp->AppendLight({ glm::vec3(1.0, 2.0, 2.0), glm::vec3(0.0,1.0,0.0), 2.0, 1.0 });

            //vulkanApp->AppendLight({ glm::vec3(0.0, 2.0, 10.0), glm::vec3(1.0,0.0,0.0), 10.0, 3.0 });
            //vulkanApp->AppendLight({ glm::vec3(0.0, 1.0, 10.0), glm::vec3(0.0,1.0,0.0), 10.0, 3.0 });

            /*
            uint32_t maxLightNumber = 5000;

            int bound = sqrt(maxLightNumber) / 2.0;

            for (int i = -bound; i < bound; ++i)
            {
                for (int j = -bound; j < bound; ++j)
                {
                    glm::vec3 color = glm::vec3(static_cast<float>(i + 5) / 10.0, 1.0 - (static_cast<float>(j + 5) / 10.0), 0.0);
                    vulkanApp->AppendLight({ glm::vec3(i, 0.5, j), color, 5.0, 1.0 });
                }
            }
            
            */

            vulkanApp->BindResources();
        }

        auto animationSystem = Core::AnimationSystemLocator::GetAnimationSystem();

        auto boneMap = animationSystem->GetMapBoneName();

        for (auto& bone : boneMap)
        {
            std::printf("BoneID: %d BoneName %s\n", bone.second.id, bone.first.c_str());
        }


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

