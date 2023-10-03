#include "pch.h"
#include "Graphics/ReinkanVulkan.h"
#include "ReinkanModelLoader.h"

#include <fstream>
#include <stack>
#include <assimp/Importer.hpp>
#include <assimp/version.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Locator/AnimationSystemLocator.h"
#include "Graphics/Structure/ObjectData.h"
#include "Animation/Structures/BoneInfo.h"
#include "Animation/Utilities/AssimpGlmHelper.h"

namespace Reinkan::Graphics
{
    void ReinkanApp::LoadModel(const ModelData& modelData, glm::mat4 transform)
    {
        ModelDataLoading modelDataLoading;
        modelDataLoading.modelDataPtr = std::make_shared<ModelData>(modelData);
        modelDataLoading.modelTransform = transform;

        appModelDataToBeLoaded.push_back(modelDataLoading);
    }

    void ReadAssimpFile(const std::string& path,
        const glm::mat4 modelTransform,
        std::vector<ModelData>& modelDatas,
        std::vector<Material>& materialPool,
        std::vector<std::string>& texturePool,
        unsigned int materialOffset)
    {
        std::printf("- - [ASSIMP]: ReadAssimpFile File:  %s \n", path.c_str());

        aiMatrix4x4 modelTr(modelTransform[0][0], modelTransform[1][0], modelTransform[2][0], modelTransform[3][0],
                            modelTransform[0][1], modelTransform[1][1], modelTransform[2][1], modelTransform[3][1],
                            modelTransform[0][2], modelTransform[1][2], modelTransform[2][2], modelTransform[3][2],
                            modelTransform[0][3], modelTransform[1][3], modelTransform[2][3], modelTransform[3][3]);

        std::ifstream find_it(path.c_str());
        if (find_it.fail())
        {
            std::cerr << "File not found: " << path << std::endl;
            exit(-1);
        }

        // Invoke assimp to read the file.
        std::printf("- - [ASSIMP]: Assimp %d.%d Reading %s\n", aiGetVersionMajor(), aiGetVersionMinor(), path.c_str());
        Assimp::Importer importer;
        const aiScene* aiscene = importer.ReadFile(path.c_str(),
                                                    aiProcess_Triangulate |
                                                    aiProcess_GenSmoothNormals |
                                                    aiProcess_CalcTangentSpace |
                                                    aiProcess_PopulateArmatureData |
                                                    aiProcess_JoinIdenticalVertices);

        if (!aiscene) 
        {
            std::printf("- - [ASSIMP]: ... Failed to read.\n");
            exit(-1);
        }

        if (!aiscene->mRootNode) 
        {
            std::printf("- - [ASSIMP]: Scene has no rootnode.\n");
            exit(-1);
        }

        std::printf("- - [ASSIMP]: Assimp mNumMeshes: %d\n", aiscene->mNumMeshes);
        std::printf("- - [ASSIMP]: Assimp mNumMaterials: %d\n", aiscene->mNumMaterials);
        std::printf("- - [ASSIMP]: Assimp mNumTextures: %d\n", aiscene->mNumTextures);

        // Load Animation
        if (aiscene->mNumAnimations > 0)
        {
            for (int i = 0; i < aiscene->mNumAnimations; ++i)
            {
                auto animation = aiscene->mAnimations[i];
                std::printf("- - [ASSIMP]: Animation %d: %s\n", i, animation->mName.C_Str());
                std::printf("- - [ASSIMP]: mDuration %f\n", animation->mDuration);
                std::printf("- - [ASSIMP]: mTicksPerSecond %f\n", animation->mTicksPerSecond);
                std::printf("- - [ASSIMP]: mNumMeshChannels %d\n", animation->mNumMeshChannels);
                
                for (int j = 0; j < animation->mNumChannels; ++j)
                {
                    std::printf("- - [ASSIMP]: Channels %d: %s\n", j, animation->mChannels[j]->mNodeName.C_Str());

                }

            }
        }

        for (int i = 0; i < aiscene->mNumMaterials; i++) 
        {
            aiMaterial* mtl = aiscene->mMaterials[i];
            aiString name;
            mtl->Get(AI_MATKEY_NAME, name);
            //std::printf("- - [ASSIMP]: Material %d: %s\n", i, name.C_Str());
            aiColor3D emit(0.f, 0.f, 0.f);
            bool he = mtl->Get(AI_MATKEY_COLOR_EMISSIVE, emit);
            aiColor3D diff(0.f, 0.f, 0.f);
            bool hd = mtl->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
            aiColor3D spec(0.f, 0.f, 0.f);
            bool hs = mtl->Get(AI_MATKEY_COLOR_SPECULAR, spec);
            float shininess = 20.0;
            bool ha = mtl->Get(AI_MATKEY_SHININESS, &shininess, NULL);
            aiColor3D trans;
            bool ht = mtl->Get(AI_MATKEY_COLOR_TRANSPARENT, trans);

            Material newmat;
            if (!emit.IsBlack()) // An emitter
            { 
                newmat.diffuse = { 1,1,1 };  // An emitter needs (1,1,1), else black screen!  WTF???
                newmat.specular = { 0,0,0 };
                newmat.shininess = 0.0;
                //newmat.emission = { emit.r, emit.g, emit.b };
                newmat.diffuseMapId = -1;
            }
            else 
            {
                ////////////////////////////////////////////////
                // Read material from mtl and make own material
                ////////////////////////////////////////////////
                glm::vec3 Kd(0.5f, 0.5f, 0.5f);
                glm::vec3 Ks(0.03f, 0.03f, 0.03f);
                if (AI_SUCCESS == hd) Kd = glm::vec3(diff.r, diff.g, diff.b);
                if (AI_SUCCESS == hs) Ks = glm::vec3(spec.r, spec.g, spec.b);
                newmat.diffuse = { Kd[0], Kd[1], Kd[2] };
                newmat.specular = { Ks[0], Ks[1], Ks[2] };
                newmat.shininess = shininess; //sqrtf(2.0f/(2.0f+shininess));
                //newmat.emission = { 0,0,0 };
                newmat.diffuseMapId = -1;
                newmat.normalMapId = -1;
                newmat.heightMapId = -1;
            }

            aiString texPath;

            // DIFFUSE
            if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)) 
            {
                std::string texturePathExtended("../assets/textures/");
                texturePathExtended += texPath.C_Str();
                newmat.diffuseMapId = texturePool.size();
                
                // use texturePool appTexturePaths
                texturePool.push_back(std::string(texturePathExtended));
                //std::printf("- - [ASSIMP]: ID: %d \tDiffuse Texture: \t%s\n", newmat.diffuseMapId, texturePathExtended.c_str());
            }

            // NORMAL
            if (AI_SUCCESS == mtl->GetTexture(aiTextureType_NORMALS, 0, &texPath))
            {
                std::string texturePathExtended("../assets/textures/");
                texturePathExtended += texPath.C_Str();
                newmat.normalMapId = texturePool.size();
                
                // use texturePool appTexturePaths
                texturePool.push_back(std::string(texturePathExtended));
                //std::printf("- - [ASSIMP]: ID: %d \tNormal Texture: \t%s\n", newmat.normalMapId, texturePathExtended.c_str());
            }

            // HEIGHT
            if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DISPLACEMENT, 0, &texPath)) 
            {
                std::string texturePathExtended("../assets/textures/");
                texturePathExtended += texPath.C_Str();
                newmat.heightMapId = texturePool.size();
                
                // use texturePool appTexturePaths
                texturePool.push_back(std::string(texturePathExtended));
                //std::printf("- - [ASSIMP]: ID: %d \tHeight(Disp) Texture: \t%s\n", newmat.heightMapId, texturePathExtended.c_str());
            }

            // change to materialPool appMaterials
            materialPool.push_back(newmat);
        }

        RecurseModelNodes(modelDatas, aiscene, aiscene->mRootNode, modelTr, 0, materialOffset);
	}

    void RecurseModelNodes(std::vector<ModelData>& modelDatas,
                           const aiScene* aiscene,
                           const aiNode* node,
                           const aiMatrix4x4& parentTr,
                           const int level,
                           unsigned int materialOffset)
    {
        // Print line with indentation to show structure of the model node hierarchy.
        //std::printf("- - [ASSIMP]: ");
        //for (int i = 0; i <= level; i++) std::printf(" |%d| ", i);
        //std::printf("%s \n", node->mName.data);

        // Accumulating transformations while traversing down the hierarchy.    
        aiMatrix4x4 childTr = parentTr * node->mTransformation;
        aiMatrix3x3 normalTr = aiMatrix3x3(childTr); // Really should be inverse-transpose for full generality

        // Loop through this node's meshes
        for (unsigned int m = 0; m < node->mNumMeshes; ++m) 
        {
            aiMesh* aimesh = aiscene->mMeshes[node->mMeshes[m]];
            //std::printf("- - [ASSIMP]:   %d: %s vert-%d: face-%d\n", m, aimesh->mName.C_Str(), aimesh->mNumVertices, aimesh->mNumFaces);

            ModelData modelDataMesh;

            modelDataMesh.name = aimesh->mName.C_Str();

            /*
            if (aimesh->mNumBones > 0)
            {
                aiBone* aibone = aimesh->mBones[0];

                ProcessBones(aibone);
            }
            */

            // Loop through all vertices and record the
            // vertex/normal/texture/tangent data with the node's model
            // transformation applied.
            unsigned int faceOffset = modelDataMesh.vertices.size();
            for (unsigned int t = 0; t < aimesh->mNumVertices; ++t)
            {
                aiVector3D aipnt = childTr * aimesh->mVertices[t];
                aiVector3D ainrm = aimesh->HasNormals() ? normalTr * aimesh->mNormals[t] : aiVector3D(0, 0, 1);
                aiVector3D aitex = aimesh->HasTextureCoords(0) ? aimesh->mTextureCoords[0][t] : aiVector3D(0, 0, 0);
                aiVector3D aitan = aimesh->HasTangentsAndBitangents() ? normalTr * aimesh->mTangents[t] : aiVector3D(1, 0, 0);
                aiVector3D aibit = aimesh->HasTangentsAndBitangents() ? normalTr * aimesh->mBitangents[t] : aiVector3D(1, 0, 0);

                Vertex vertex;
                vertex.position = { aipnt.x, aipnt.y, aipnt.z };
                vertex.vertexNormal = { ainrm.x, ainrm.y, ainrm.z };
                vertex.vertexTangent = { aitan.x, aitan.y, aitan.z };
                vertex.vertexBitangent = { aibit.x, aibit.y, aibit.z };
                vertex.texCoord = { aitex.x , 1.0 - aitex.y };
                // The OBJ format assumes a coordinate system 
                // where a vertical coordinate of 0 means the bottom of the image, 
                // however we've uploaded our image into Vulkan in a top to bottom orientation 
                // where 0 means the top of the image. Solve this by 
                // flipping the vertical component of the texture coordinates

                auto animationSystem = Core::AnimationSystemLocator::GetAnimationSystem();

                for (int boneIndex = 0; boneIndex < std::min(aimesh->mNumBones, MAX_BONE_INFLUENCE); ++boneIndex)
                {
                    vertex.boneIds[boneIndex] = animationSystem->GetBoneId(aimesh->mBones[boneIndex]->mName.C_Str());
                    //vertex.boneWeights[boneIndex] = aimesh->mBones[boneIndex]->mWeights->

                    //vertex.boneWeights[boneIndex] = -1;
                    //bones[boneIndex] = aimesh->mBones[boneIndex]->mNode->;
                    //weights[boneIndex] = aimesh->mBones[boneIndex]->mWeights
                }

                modelDataMesh.vertices.push_back(vertex);
            }
                                       

            // force mesh to have only 1 material
            modelDataMesh.materialIndex = materialOffset + aimesh->mMaterialIndex;
            //std::printf("meshName: %s\n", aimesh->mName.C_Str());
            //std::printf("matId: %d size: \n", modelDataMesh.materialIndex);

            // Loop through all faces, recording indices
            for (unsigned int t = 0; t < aimesh->mNumFaces; ++t)
            {
                aiFace* aiface = &aimesh->mFaces[t];
                if (aiface->mNumIndices > 3)
                {
                    //std::printf("- - [ASSIMP]: indices > 3  mNumIndices: %d\n", aiface->mNumIndices);
                }
                for (int i = 2; i < aiface->mNumIndices; i++) 
                {
                    modelDataMesh.indices.push_back(aiface->mIndices[0] + faceOffset);
                    modelDataMesh.indices.push_back(aiface->mIndices[i - 1] + faceOffset);
                    modelDataMesh.indices.push_back(aiface->mIndices[i] + faceOffset);
                }
            };

            ExtractBoneWeightForVertices(modelDataMesh.vertices, aimesh);

            modelDatas.push_back(modelDataMesh);
        }

        // Recurse onto this node's children
        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            RecurseModelNodes(modelDatas, aiscene, node->mChildren[i], childTr, level + 1, materialOffset);
        }
    }

    void ProcessBones(aiBone* bone)
    {
        //DFS
        std::stack<aiNode*> dfsStack;
        dfsStack.push(bone->mNode);

        std::string level = "| ";

        while (!dfsStack.empty())
        {
            aiNode* currentNode = dfsStack.top();
            dfsStack.pop();

            std::printf("- - [ASSIMP]: [BONE]: %s Name %s\n", level.c_str(), currentNode->mName.C_Str());

            // Process 

            for (int i = 0; i < currentNode->mNumChildren; ++i)
            {
                dfsStack.push(currentNode->mChildren[i]);
            }
        }
    }

    void SetVertexBoneData(Vertex& vertex, int boneID, float weight)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
        {
            if (vertex.boneIds[i] < 0)
            {
                vertex.boneWeights[i] = weight;
                vertex.boneIds[i] = boneID;
                break;
            }
        }
    }

    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh)
    {
        auto animationSystem = Core::AnimationSystemLocator::GetAnimationSystem();
        auto& boneInfoMap = animationSystem->GetMapBoneName();

        for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
        {
            //int boneID = -1;
            std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
            
            uint32_t boneId = animationSystem->GetBoneId(boneName);

            if (boneId != -1)
            {
                std::printf("Duplicated Bone #%d : %s\n", boneId, boneName.c_str());
            }

            if (boneId == -1)
            {
                animationSystem->AddBoneName(boneName);

                auto offset = Utilities::AssimpGlmHelper::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);;
                animationSystem->SetBoneOffset(boneName, offset);

                boneId = animationSystem->GetBoneId(boneName);
            }
            assert(boneId != -1);

            auto weights = mesh->mBones[boneIndex]->mWeights;
            int numWeights = mesh->mBones[boneIndex]->mNumWeights;

            for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
            {
                int vertexId = weights[weightIndex].mVertexId;
                float weight = weights[weightIndex].mWeight;
                assert(vertexId <= vertices.size());
                SetVertexBoneData(vertices[vertexId], boneId, weight);
            }
        }
    }
}