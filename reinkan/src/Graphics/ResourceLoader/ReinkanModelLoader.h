#pragma once


#include <assimp/Importer.hpp>
#include <assimp/version.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Graphics/Structure/ModelData.h"
#include "Graphics/Structure/ObjectData.h"

namespace Reinkan::Graphics
{
    void ReadAssimpFile(const std::string& path,
        const glm::mat4 mat,
        std::vector<ModelData>& modelData,
        std::vector<Material>& materialPool,
        std::vector<std::string>& texturePool,
        unsigned int materialOffset);

    void RecurseModelNodes(std::vector<ModelData>& modelData,
        const aiScene* aiscene,
        const aiNode* node,
        const aiMatrix4x4& parentTr,
        const int level,
        unsigned int materialOffset);

    //void ProcessBones(aiBone* bone);

    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh);
}