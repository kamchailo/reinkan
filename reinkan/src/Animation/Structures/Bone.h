#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
// Change to Engine Quaternion
#include <glm/gtc/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/version.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/anim.h>

#include "Animation/Structures/Keyframe.h"
#include "Animation/Utilities/AssimpGlmHelper.h"

namespace Reinkan::Animation
{
    class Bone
    {
    public:
        
        Bone(const std::string& name, int ID, const aiNodeAnim* channel);
        
        void Update(float animationTime);

        glm::mat4 GetLocalTransform() { return localTransform; }

        std::string GetBoneName() const { return name; }
        
        int GetBoneID() { return id; }
        
        int GetPositionIndex(float animationTime);

        int GetRotationIndex(float animationTime);

        int GetScaleIndex(float animationTime);

    private:
        
        float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
        
        glm::mat4 InterpolatePosition(float animationTime);

        glm::mat4 InterpolateRotation(float animationTime);

        glm::mat4 InterpolateRotationGLM(float animationTime);

        glm::mat4 InterpolateScaling(float animationTime);

        std::vector<KeyPosition> positions;

        std::vector<KeyRotation> rotations;

        std::vector<KeyScale> scales;

        int numPositions;

        int numRotations;

        int numScalings;

        glm::mat4 localTransform;

        std::string name;

        int id;
    };
}