#include "pch.h"

#include "Reinkan.h"
#include "ResourceLoader/ReinkanModelLoader.h"
#include "Structure/ModelData.h"

#include <glm/gtc/matrix_transform.hpp>

int main() 
{
    ////////////////////////////////////////
    //          Vulkan Init
    ////////////////////////////////////////
    Reinkan::ReinkanApp app(800,600);
    app.Init();

    ////////////////////////////////////////
    //          Load & Bind Resources
    ////////////////////////////////////////
    {   // Indent to free model data after load

        std::vector<Reinkan::ModelData> modelDatas;

        //Reinkan::ModelData model;
        Reinkan::ReadAssimpFile("../assets/models/simpleshape.obj", 
                                glm::mat4(1.0), 
                                modelDatas,
                                app.GetAppMaterialPool(), 
                                app.GetAppTexturePool(), 
                                app.GetAppMaterialPool().size());

        //Reinkan::ModelData vampire;
        Reinkan::ReadAssimpFile("../assets/models/dancing_vampire.dae", 
                                glm::mat4(1.0), 
                                modelDatas,
                                app.GetAppMaterialPool(), 
                                app.GetAppTexturePool(), 
                                app.GetAppMaterialPool().size());

        //Reinkan::ModelData sponza;
        Reinkan::ReadAssimpFile("../assets/models/sponza.obj",
                                glm::mat4(0.01),
                                modelDatas,
                                app.GetAppMaterialPool(),
                                app.GetAppTexturePool(),
                                app.GetAppMaterialPool().size());

        glm::mat4 modelTr = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, 0.0));
        //glm::mat4 vampireTr = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, 0.0));
        //glm::mat4 sponzaTr = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, 0.0));
        //app.LoadModel(std::make_shared<Reinkan::ModelData>(model), modelTr);
        //app.LoadModel(std::make_shared<Reinkan::ModelData>(vampire), vampireTr);
        //app.LoadModel(std::make_shared<Reinkan::ModelData>(sponza), sponzaTr);

        for (int i = 0; i < modelDatas.size(); ++i)
        {
            app.LoadModel(std::make_shared<Reinkan::ModelData>(modelDatas[i]), modelTr);
        }


        // Lights
        app.AppendLight({ glm::vec3(2.0, 1.0, 1.0), glm::vec3(1.0,0.0,0.0), 1.0, 1.0 });

        app.BindResources();
    }

    ////////////////////////////////////////
    //          Game Object Control
    ////////////////////////////////////////
    app.SetEyePosition(0.0, 5.0, 5.0);

    ////////////////////////////////////////
    //          Vulkan Main Program
    ////////////////////////////////////////
    try 
    {
        //app.Run();
        //app.MainLoop();

        while (!app.ShouldClose())
        {
            app.ReinkanUpdate();

        }

        app.Cleanup();

    }
    catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

    return 0;
}