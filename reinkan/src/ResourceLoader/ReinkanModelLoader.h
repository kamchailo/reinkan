#pragma once


#include <assimp/Importer.hpp>
#include <assimp/version.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Structure/ModelData.h"
#include "Structure/ObjectData.h"

namespace Reinkan
{
    void ReadAssimpFile(const std::string& path,
        const glm::mat4 mat,
        ModelData& modelData,
        std::vector<Material>& materialPool,
        std::vector<std::string>& texturePool,
        unsigned int materialOffset);

    void RecurseModelNodes(ModelData& modelData,
        const aiScene* aiscene,
        const aiNode* node,
        const aiMatrix4x4& parentTr,
        const int level,
        unsigned int materialOffset);
}