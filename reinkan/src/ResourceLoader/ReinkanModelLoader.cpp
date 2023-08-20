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

    void ReadAssimpFile(const std::string& path,
                        const glm::mat4 modelTransform, 
                        ModelData& modelData, 
                        std::vector<Material>& materialPool,
                        std::vector<std::string>& texturePool,
                        unsigned int materialOffset)
	{
        std::printf("- - [ASSIMP]: ReadAssimpFile File:  %s \n", path.c_str());

        aiMatrix4x4 modelTr(modelTransform[0][0], modelTransform[1][0], modelTransform[2][0], modelTransform[3][0],
                            modelTransform[0][1], modelTransform[1][1], modelTransform[2][1], modelTransform[3][1],
                            modelTransform[0][2], modelTransform[1][2], modelTransform[2][2], modelTransform[3][2],
                            modelTransform[0][3], modelTransform[1][3], modelTransform[2][3], modelTransform[3][3]);

        // Does the file exist?
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
            aiProcess_Triangulate | aiProcess_GenSmoothNormals);

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

        // Check Animation
        if (aiscene->mNumAnimations > 0)
        {
            for (int i = 0; i < aiscene->mNumAnimations; ++i)
            {
                std::printf("- - [ASSIMP]: Animation %d: %s\n", i, aiscene->mAnimations[i]->mName.C_Str());
                // Load Animation
            }
        }

        for (int i = 0; i < aiscene->mNumMaterials; i++) 
        {
            aiMaterial* mtl = aiscene->mMaterials[i];
            aiString name;
            mtl->Get(AI_MATKEY_NAME, name);
            std::printf("- - [ASSIMP]: Material %d: %s\n", i, name.C_Str());
            aiColor3D emit(0.f, 0.f, 0.f);
            bool he = mtl->Get(AI_MATKEY_COLOR_EMISSIVE, emit);
            aiColor3D diff(0.f, 0.f, 0.f);
            bool hd = mtl->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
            aiColor3D spec(0.f, 0.f, 0.f);
            bool hs = mtl->Get(AI_MATKEY_COLOR_SPECULAR, spec);
            float alpha = 20.0;
            bool ha = mtl->Get(AI_MATKEY_SHININESS, &alpha, NULL);
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
                newmat.shininess = alpha; //sqrtf(2.0f/(2.0f+alpha));
                //newmat.emission = { 0,0,0 };
                newmat.diffuseMapId = -1;
            }

            aiString texPath;

            // DIFFUSE
            if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)) 
            {
                std::string texturePathExtended("../assets/textures/");
                texturePathExtended += texPath.C_Str();
                //newmat.diffuseMapId = modelData.textures.size();
                newmat.diffuseMapId = texturePool.size();
                // use texturePool appTexturePaths
                //modelData.textures.push_back(std::string(texPath.C_Str()));
                texturePool.push_back(std::string(texturePathExtended));
                std::printf("- - [ASSIMP]: ID: %d \tDiffuse Texture: \t%s\n", newmat.diffuseMapId, texturePathExtended.c_str());
            }

            // NORMAL
            if (AI_SUCCESS == mtl->GetTexture(aiTextureType_NORMALS, 0, &texPath))
            {
                std::string texturePathExtended("../assets/textures/");
                texturePathExtended += texPath.C_Str();
                //newmat.normalMapId = modelData.textures.size();
                newmat.normalMapId = texturePool.size();
                // use texturePool appTexturePaths
                texturePool.push_back(std::string(texturePathExtended));
                std::printf("- - [ASSIMP]: ID: %d \tNormal Texture: \t%s\n", newmat.normalMapId, texturePathExtended.c_str());
            }

            // HEIGHT
            if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DISPLACEMENT, 0, &texPath)) 
            {
                std::string texturePathExtended("../assets/textures/");
                texturePathExtended += texPath.C_Str();
                //newmat.normalMapId = modelData.texture
                //newmat.heightMapId = modelData.textures.size();
                newmat.heightMapId = texturePool.size();
                // use texturePool appTexturePaths
                texturePool.push_back(std::string(texturePathExtended));
                std::printf("- - [ASSIMP]: ID: %d \tHeight(Disp) Texture: \t%s\n", newmat.heightMapId, texturePathExtended.c_str());
            }

            // change to materialPool appMaterials
            //modelData.materials.push_back(newmat);
            materialPool.push_back(newmat);
        }

        RecurseModelNodes(modelData, aiscene, aiscene->mRootNode, modelTr, 0, materialOffset);

	}
    void RecurseModelNodes(ModelData& modelData,
                           const aiScene* aiscene,
                           const aiNode* node,
                           const aiMatrix4x4& parentTr,
                           const int level,
                           unsigned int materialOffset)
    {
        // Print line with indentation to show structure of the model node hierarchy.
        std::printf("- - [ASSIMP]: ");
        for (int i = 0; i <= level; i++) std::printf(" |%d| ", i);
        std::printf("%s \n", node->mName.data);

        // Accumulating transformations while traversing down the hierarchy.
        aiMatrix4x4 childTr = parentTr * node->mTransformation;
        aiMatrix3x3 normalTr = aiMatrix3x3(childTr); // Really should be inverse-transpose for full generality

        // Loop through this node's meshes
        for (unsigned int m = 0; m < node->mNumMeshes; ++m) {
            aiMesh* aimesh = aiscene->mMeshes[node->mMeshes[m]];
            std::printf("- - [ASSIMP]:   %d: vert-%d: face-%d\n", m, aimesh->mNumVertices, aimesh->mNumFaces);

            // Load Bone
            for (int boneIndex = 0; boneIndex < aimesh->mNumBones; ++boneIndex)
            {
                aiBone* aibone = aimesh->mBones[boneIndex];
                //std::printf("- - [ASSIMP]: #%d Bone: %s mNumWeights: %d \n", aibone->mNode, aibone->mName.C_Str(), aibone->mNumWeights);
                for (int vertW = 0; vertW < aibone->mNumWeights; ++vertW)
                {
                    //std::printf("- - [ASSIMP]:   VertID: %d Weight: %f\n", aibone->mWeights[vertW].mVertexId, aibone->mWeights[vertW].mWeight);
                }
            }

            // Loop through all vertices and record the
            // vertex/normal/texture/tangent data with the node's model
            // transformation applied.
            unsigned int faceOffset = modelData.vertices.size();
            for (unsigned int t = 0; t < aimesh->mNumVertices; ++t) 
            {
                aiVector3D aipnt = childTr * aimesh->mVertices[t];
                aiVector3D ainrm = aimesh->HasNormals() ? normalTr * aimesh->mNormals[t] : aiVector3D(0, 0, 1);
                aiVector3D aitex = aimesh->HasTextureCoords(0) ? aimesh->mTextureCoords[0][t] : aiVector3D(0, 0, 0);
                aiVector3D aitan = aimesh->HasTangentsAndBitangents() ? normalTr * aimesh->mTangents[t] : aiVector3D(1, 0, 0);

                // Model doesn't have tangent information
                if (aimesh->HasTangentsAndBitangents())
                {
                    std::printf("I have Tangent\n");
;                }

                modelData.vertices.push_back({ {aipnt.x, aipnt.y, aipnt.z},
                                               {ainrm.x, ainrm.y, ainrm.z},
                                               {aitan.x, aitan.y, aitan.z},
                                               {aitex.x , 1.0 - aitex.y} }); 
                                            // The OBJ format assumes a coordinate system 
                                            // where a vertical coordinate of 0 means the bottom of the image, 
                                            // however we've uploaded our image into Vulkan in a top to bottom orientation 
                                            // where 0 means the top of the image. Solve this by 
                                            // flipping the vertical component of the texture coordinates
            }

            // Loop through all faces, recording indices
            for (unsigned int t = 0; t < aimesh->mNumFaces; ++t) 
            {
                aiFace* aiface = &aimesh->mFaces[t];
                for (int i = 2; i < aiface->mNumIndices; i++) {
                    //modelData.materialIndex.push_back(aimesh->mMaterialIndex);
                    modelData.materialIndex.push_back(materialOffset + aimesh->mMaterialIndex);
                    //std::printf("matId: %d size: %d\n", modelData.materialIndex[modelData.materialIndex.size() - 1], modelData.materialIndex.size());
                    
                    modelData.indices.push_back(aiface->mIndices[0] + faceOffset);
                    modelData.indices.push_back(aiface->mIndices[i - 1] + faceOffset);
                    modelData.indices.push_back(aiface->mIndices[i] + faceOffset);
                }
            };
        }

        // Recurse onto this node's children
        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            RecurseModelNodes(modelData, aiscene, node->mChildren[i], childTr, level + 1, materialOffset);
        }
    }
}