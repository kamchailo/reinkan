#include "pch.h"
#include "GraphicsSystem.h"

#include <random>
#include <glm/gtx/transform.hpp>

#include "ResourceLoader/ReinkanModelLoader.h"

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


            glm::mat4 modelTr = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, 0.0));
            //ModelData walking;
            ReadAssimpFile("../assets/models/Walking.dae",
                modelTr,
                modelDatas,
                vulkanApp->GetAppMaterialPool(),
                vulkanApp->GetAppTexturePool(),
                vulkanApp->GetAppMaterialPool().size());
            
            */
            glm::mat4 planeTr = glm::translate(glm::mat4(1), glm::vec3(0.0, 1.0, 0.0));
            //ModelData plane;
            ReadAssimpFile("../assets/models/plane.obj",
                planeTr,
                modelDatas,
                vulkanApp->GetAppMaterialPool(),
                vulkanApp->GetAppTexturePool(),
                vulkanApp->GetAppMaterialPool().size());

            /*
            //ModelData sponza;
            ReadAssimpFile("../assets/models/sponza.obj",
                glm::mat4(0.01),
                modelDatas,
                vulkanApp->GetAppMaterialPool(),
                vulkanApp->GetAppTexturePool(),
                vulkanApp->GetAppMaterialPool().size());
 
            */

            for (int i = 0; i < modelDatas.size(); ++i)
            {
                vulkanApp->LoadModel(modelDatas[i], glm::mat4(1.0f));
            }


            // Lights
            vulkanApp->AppendLight({ glm::vec3(-1.0, 2.0, 0.0), glm::vec3(1.0,0.0,0.0), 2.0, 1.0 });
            vulkanApp->AppendLight({ glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0,1.0,0.0), 2.0, 1.0 });
            vulkanApp->AppendLight({ glm::vec3(1.0, 2.0, 0.0), glm::vec3(0.0,1.0,0.0), 2.0, 1.0 });
            /*
            uint32_t maxLightNumber = 5000;

            int bound = sqrt(maxLightNumber) / 2.0;

            int boundX = 16;
            int boundZ = 8;
            int x, y ,z;

            for (int i = -bound; i < bound; ++i)
            {
                for (int j = -bound; j < bound; ++j)
                {
                    x = i % boundX;
                    y = rand() % 3;
                    z = j % boundZ;
                    glm::vec3 color = glm::vec3(static_cast<float>(abs(x) + 5) / 10.0, 1.0 - (static_cast<float>(abs(z) + 5) / 10.0), 0.0);
                    
                    vulkanApp->AppendLight({ glm::vec3(x, y, z), color, 5.0, 1.0 });
                }
            }
            */

            vulkanApp->AddPyramidalPath("../assets/pyramidal/simple_hill_out_0.png");
            vulkanApp->AddPyramidalPath("../assets/pyramidal/simple_hill_out_1.png");
            vulkanApp->AddPyramidalPath("../assets/pyramidal/simple_hill_out_2.png");
            vulkanApp->AddPyramidalPath("../assets/pyramidal/simple_hill_out_3.png");
            vulkanApp->AddPyramidalPath("../assets/pyramidal/simple_hill_out_4.png");
            //vulkanApp->AddPyramidalPath("../assets/pyramidal/ceiling-flower_out_5.png");
            //vulkanApp->AddPyramidalPath("../assets/pyramidal/ceiling-flower_out_6.png");
            //vulkanApp->AddPyramidalPath("../assets/pyramidal/ceiling-flower_out_7.png");
            //vulkanApp->AddPyramidalPath("../assets/pyramidal/ceiling-flower_out_8.png");
            //vulkanApp->AddPyramidalPath("../assets/pyramidal/ceiling-flower_out_9.png");
            //vulkanApp->AddPyramidalPath("../assets/pyramidal/ceiling-flower_out_10.png");
            //vulkanApp->AddPyramidalPath("../assets/pyramidal/ceiling-flower_out_11.png");
            

            vulkanApp->BindResources();
        }

        ////////////////////////////////////////
        //          Game Object Control
        ////////////////////////////////////////
        //vulkanApp->SetEyePosition(0.0, 3.0, 2.0);

        // set binding to action
        // CreateAction 
        // action call camera

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

