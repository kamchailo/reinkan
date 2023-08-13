#include "pch.h"

#include "Reinkan.h"
#include "Loader/ReinkanModelLoader.h"
#include "Structure/ModelData.h"

#include <glm/gtc/matrix_transform.hpp>

int main() 
{
    Reinkan::ReinkanApp app(800,600);

    app.Init();

    // Load Resources
    {
        Reinkan::ModelData model;
        Reinkan::ReadAssimpFile("../assets/models/simpleshape.obj", glm::mat4(0.1), model);
        Reinkan::ModelData vampire;
        Reinkan::ReadAssimpFile("../assets/models/dancing_vampire.dae", glm::mat4(0.1), vampire);

        glm::mat4 vampireTr = glm::translate(glm::mat4(1), glm::vec3(1.0, 0.0, 0.0));

        app.LoadModel(std::make_shared<Reinkan::ModelData>(model), glm::mat4(1));
        app.LoadModel(std::make_shared<Reinkan::ModelData>(vampire), vampireTr);

        app.BindResources();

    }

    try 
    {
        app.Run();
    }
    catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

    return 0;
}