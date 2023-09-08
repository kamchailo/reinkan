#include "pch.h"

#include "Core/ReinkanEngine.h"

#include "Graphics/ReinkanVulkan.h"
#include "ResourceLoader/ReinkanModelLoader.h"
#include "Graphics/Structure/ModelData.h"

#include <glm/gtc/matrix_transform.hpp>

int main() 
{
    Reinkan::Core::ReinkanEngine engine;

    engine.Init();
    
    engine.Update();

    engine.Destroy();

    return 0;
}