#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "Animation/Structures/BoneInfo.h"
#include "Animation/Structures/Bone.h"
#include "Animation/Utilities/AssimpGlmHelper.h"

namespace Reinkan::Animation
{
    struct AssimpNodeData
    {
        glm::mat4 transformation;

        std::string name;
        
        int childrenCount;
        
        std::vector<AssimpNodeData> children;

        AssimpNodeData* parent;
    };


	class Animation
	{
    public:
        Animation() = default;

        //Animation(const std::string& animationPath, Model* model)
        Animation(const std::string& animationPath);

        ~Animation() {}

        Bone* FindBone(const std::string& name);

        inline float GetTicksPerSecond() { return m_TicksPerSecond; }

        inline float GetDuration() { return m_Duration; }

        inline const AssimpNodeData& GetRootNode() { return m_RootNode; }

        inline const std::map<std::string, BoneInfo>& GetBoneIDMap() { return m_BoneInfoMap; }

    private:
        //void ReadMissingBones(const aiAnimation* animation, Model& model) // support more than one bone structure
        void ReadMissingBones(const aiAnimation* animation);

        void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src, AssimpNodeData* parent);

        float m_Duration;

        int m_TicksPerSecond;

        std::vector<Bone> m_Bones;
        
        AssimpNodeData m_RootNode;
        
        std::map<std::string, BoneInfo> m_BoneInfoMap;
	};
}