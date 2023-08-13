#include "pch.h"
#include "Reinkan.h"
#include "ReinkanModelLoader.h"

#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/version.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Structure/ObjectData.h"

namespace Reinkan
{
    void ReinkanApp::LoadModel(std::shared_ptr<ModelData> modelData, glm::mat4 transform)
    {
        appModelDataToBeLoaded.push_back(std::pair<std::shared_ptr<ModelData>, glm::mat4>(modelData, transform));
    }

    void Reinkan::ReinkanApp::BindModelData()
    {
        for (auto modelData : appModelDataToBeLoaded)
        {
            ObjectData object;
            object.nbVertices = modelData.first->vertices.size();
            object.nbIndices = modelData.first->indices.size();

            object.transform = modelData.second;

            object.vertexBuffer = CreateStagedBufferWrap(modelData.first->vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            object.indexBuffer = CreateStagedBufferWrap(modelData.first->indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

            appObjects.push_back(object);
        }
    }

    void ReadAssimpFile(const std::string& path, const glm::mat4 mat, ModelData& modelData)
	{
        printf("- - [ASSIMP]: ReadAssimpFile File:  %s \n", path.c_str());

        aiMatrix4x4 modelTr(mat[0][0], mat[1][0], mat[2][0], mat[3][0],
                            mat[0][1], mat[1][1], mat[2][1], mat[3][1],
                            mat[0][2], mat[1][2], mat[2][2], mat[3][2],
                            mat[0][3], mat[1][3], mat[2][3], mat[3][3]);

        // Does the file exist?
        std::ifstream find_it(path.c_str());
        if (find_it.fail()) {
            std::cerr << "File not found: " << path << std::endl;
            exit(-1);
        }

        // Invoke assimp to read the file.
        printf("- - [ASSIMP]: Assimp %d.%d Reading %s\n", aiGetVersionMajor(), aiGetVersionMinor(), path.c_str());
        Assimp::Importer importer;
        const aiScene* aiscene = importer.ReadFile(path.c_str(),
            aiProcess_Triangulate | aiProcess_GenSmoothNormals);

        if (!aiscene) {
            printf("- - [ASSIMP]: ... Failed to read.\n");
            exit(-1);
        }

        if (!aiscene->mRootNode) {
            printf("- - [ASSIMP]: Scene has no rootnode.\n");
            exit(-1);
        }

        printf("- - [ASSIMP]: Assimp mNumMeshes: %d\n", aiscene->mNumMeshes);
        printf("- - [ASSIMP]: Assimp mNumMaterials: %d\n", aiscene->mNumMaterials);
        printf("- - [ASSIMP]: Assimp mNumTextures: %d\n", aiscene->mNumTextures);

        for (int i = 0; i < aiscene->mNumMaterials; i++) {
            aiMaterial* mtl = aiscene->mMaterials[i];
            aiString name;
            mtl->Get(AI_MATKEY_NAME, name);
            aiColor3D emit(0.f, 0.f, 0.f);
            aiColor3D diff(0.f, 0.f, 0.f), spec(0.f, 0.f, 0.f);
            float alpha = 20.0;
            bool he = mtl->Get(AI_MATKEY_COLOR_EMISSIVE, emit);
            bool hd = mtl->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
            bool hs = mtl->Get(AI_MATKEY_COLOR_SPECULAR, spec);
            bool ha = mtl->Get(AI_MATKEY_SHININESS, &alpha, NULL);
            aiColor3D trans;
            bool ht = mtl->Get(AI_MATKEY_COLOR_TRANSPARENT, trans);

            Material newmat;
            if (!emit.IsBlack()) { // An emitter
                newmat.diffuse = { 1,1,1 };  // An emitter needs (1,1,1), else black screen!  WTF???
                newmat.specular = { 0,0,0 };
                newmat.shininess = 0.0;
                //newmat.emission = { emit.r, emit.g, emit.b };
                newmat.textureId = -1;
            }

            else {
                ////////////////////////////////////////////////
                // Read material from mtl and make own material
                ////////////////////////////////////////////////
                glm::vec3 Kd(0.5f, 0.5f, 0.5f);
                glm::vec3 Ks(0.03f, 0.03f, 0.03f);
                if (AI_SUCCESS == hd) Kd = glm::vec3(diff.r, diff.g, diff.b);
                if (AI_SUCCESS == hs) Ks = glm::vec3(spec.r, spec.g, spec.b);
                newmat.diffuse = { Kd[0], Kd[1], Kd[2] };
                newmat.specular = { Ks[0], Ks[1], Ks[2] };
                newmat.shininess = alpha; //sqrtf(2.0f/(2.0f+alpha));
                //newmat.emission = { 0,0,0 };
                newmat.textureId = -1;
            }

            aiString texPath;
            if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)) {
                printf("- - [ASSIMP]: Texture: %s\n", texPath.C_Str());
                newmat.textureId = modelData.textures.size();
                modelData.textures.push_back(std::string(texPath.C_Str()));
            }

            modelData.materials.push_back(newmat);
        }

        RecurseModelNodes(modelData, aiscene, aiscene->mRootNode, modelTr);

	}

    void RecurseModelNodes(ModelData& modelData,
                           const aiScene* aiscene, 
                           const aiNode* node, 
                           const aiMatrix4x4& parentTr, 
                           const int level)
    {
        // Print line with indentation to show structure of the model node hierarchy.
        printf("- - [ASSIMP]: ");
        for (int i = 0; i < level; i++) printf(" | ");
        printf("%s \n", node->mName.data);

        // Accumulating transformations while traversing down the hierarchy.
        aiMatrix4x4 childTr = parentTr * node->mTransformation;
        aiMatrix3x3 normalTr = aiMatrix3x3(childTr); // Really should be inverse-transpose for full generality

        // Loop through this node's meshes
        for (unsigned int m = 0; m < node->mNumMeshes; ++m) {
            aiMesh* aimesh = aiscene->mMeshes[node->mMeshes[m]];
            printf("- - [ASSIMP]:   %d: vert-%d: face-%d\n", m, aimesh->mNumVertices, aimesh->mNumFaces);

            // Load Bone
            for (int boneIndex = 0; boneIndex < aimesh->mNumBones; ++boneIndex)
            {
                aiBone* aibone = aimesh->mBones[boneIndex];
                printf("- - [ASSIMP]: #%d Bone: %s weight: %d \n", aibone->mNode, aibone->mName.C_Str(), aibone->mNumWeights);
                for (int vertW = 0; vertW < aibone->mNumWeights; ++vertW)
                {
                    //printf("- - [ASSIMP]:   VertID: %d Weight: %f\n", aibone->mWeights[vertW].mVertexId, aibone->mWeights[vertW].mWeight);
                }
            }

            // Loop through all vertices and record the
            // vertex/normal/texture/tangent data with the node's model
            // transformation applied.
            unsigned int faceOffset = modelData.vertices.size();
            for (unsigned int t = 0; t < aimesh->mNumVertices; ++t) {
                aiVector3D aipnt = childTr * aimesh->mVertices[t];
                aiVector3D ainrm = aimesh->HasNormals() ? normalTr * aimesh->mNormals[t] : aiVector3D(0, 0, 1);
                aiVector3D aitex = aimesh->HasTextureCoords(0) ? aimesh->mTextureCoords[0][t] : aiVector3D(0, 0, 0);
                aiVector3D aitan = aimesh->HasTangentsAndBitangents() ? normalTr * aimesh->mTangents[t] : aiVector3D(1, 0, 0);


                modelData.vertices.push_back({ {aipnt.x, aipnt.y, aipnt.z},
                                              {ainrm.x, ainrm.y, ainrm.z},
                                              {aitex.x, aitex.y} });
            }

            // Loop through all faces, recording indices
            for (unsigned int t = 0; t < aimesh->mNumFaces; ++t) {
                aiFace* aiface = &aimesh->mFaces[t];
                for (int i = 2; i < aiface->mNumIndices; i++) {
                    modelData.materialIndex.push_back(aimesh->mMaterialIndex);
                    modelData.indices.push_back(aiface->mIndices[0] + faceOffset);
                    modelData.indices.push_back(aiface->mIndices[i - 1] + faceOffset);
                    modelData.indices.push_back(aiface->mIndices[i] + faceOffset);
                }
            };
        }

        // Recurse onto this node's children
        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            RecurseModelNodes(modelData, aiscene, node->mChildren[i], childTr, level + 1);
        }
    }
}