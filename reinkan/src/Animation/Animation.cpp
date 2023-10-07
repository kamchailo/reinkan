#include "pch.h"
#include "Animation.h"

#include "Core/Locator/AnimationSystemLocator.h"

namespace Reinkan::Animation
{
	Animation::Animation(const std::string& animationPath)
	{
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);

        assert(scene && scene->mRootNode);
        
        auto animation = scene->mAnimations[0];
        
        m_Duration = animation->mDuration;

        m_TicksPerSecond = animation->mTicksPerSecond;

        ReadHeirarchyData(m_RootNode, scene->mRootNode, nullptr);
        
        ReadMissingBones(animation);
	}

    Bone* Animation::FindBone(const std::string& name)
    {
        auto iter = std::find_if(m_Bones.begin(), 
                                 m_Bones.end(),
                                 [&](const Bone& Bone) { return Bone.GetBoneName() == name; } );

        if (iter == m_Bones.end())
        {
            return nullptr;
        }
        else
        {
            return &(*iter);
        }
    }

    void Animation::ReadMissingBones(const aiAnimation* animation)
    {
        auto animationSystem = Core::AnimationSystemLocator::GetAnimationSystem();

        int size = animation->mNumChannels;

        auto& boneInfoMap = animationSystem->GetMapBoneName(); // getting m_BoneInfoMap from Animation System
        //int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

        //reading channels(bones engaged in an animation and their keyframes)
        for (int i = 0; i < size; i++)
        {
            auto channel = animation->mChannels[i];
            std::string boneName = channel->mNodeName.data;

            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                animationSystem->AddBoneName(boneName);
            }
            m_Bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
        }

        m_BoneInfoMap = boneInfoMap;
    }

    void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src, AssimpNodeData* parent)
    {
        assert(src);

        dest.name = src->mName.data;
        dest.parent = parent;

        dest.transformation = Utilities::AssimpGlmHelper::ConvertMatrixToGLMFormat(src->mTransformation);

        // Decompose Assimp Matrix to VQS
        aiVector3D aiScale;
        aiQuaternion aiRotation;
        aiVector3D aiPosition;
        src->mTransformation.Decompose(aiScale, aiRotation, aiPosition);
        
        // Get only one value 
        // VQS only support uniform scaling
        dest.vqs.s = aiScale.y;
        dest.vqs.q = Math::Quaternion(aiRotation.w, aiRotation.x, aiRotation.y, aiRotation.z);
        dest.vqs.v = Utilities::AssimpGlmHelper::GetGLMVec(aiPosition);

        dest.childrenCount = src->mNumChildren;

        for (int i = 0; i < src->mNumChildren; i++)
        {
            AssimpNodeData newData;
            ReadHeirarchyData(newData, src->mChildren[i], &dest);
            dest.children.push_back(newData);
        }
    }
}