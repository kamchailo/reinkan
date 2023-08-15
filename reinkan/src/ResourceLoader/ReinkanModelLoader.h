#pragma once

#include <string>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "../shaders/SharedStruct.h"
#include "Structure/ModelData.h"

namespace Reinkan
{
	void ReadAssimpFile(const std::string& path, 
                        const glm::mat4 mat, 
                        ModelData& modelData,
                        std::vector<Material>& materialPool,
                        std::vector<std::string>& texturePool);

    void RecurseModelNodes(ModelData& modelData,
                           const aiScene* aiscene,
                           const aiNode* node,
                           const aiMatrix4x4& parentTr,
                           const int level = 0);
}