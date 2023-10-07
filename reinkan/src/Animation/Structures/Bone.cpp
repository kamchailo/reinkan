#include "pch.h"
#include "Bone.h"

#include "Math/Quaternion.h"
#include "Math/Interpolation.hpp"

namespace Reinkan::Animation
{
    // reads keyframes from aiNodeAnim
    Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
        :
        name(name),
        id(ID),
        localTransform(1.0f)
    {
        numPositions = channel->mNumPositionKeys;

        for (int positionIndex = 0; positionIndex < numPositions; ++positionIndex)
        {
            aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
            float timeStamp = channel->mPositionKeys[positionIndex].mTime;
            KeyPosition data;
            data.position = Utilities::AssimpGlmHelper::GetGLMVec(aiPosition);
            data.timeStamp = timeStamp;
            positions.push_back(data);
        }

        numRotations = channel->mNumRotationKeys;
        for (int rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex)
        {
            aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
            float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
            KeyRotation data;
            // Use Engine Quaternion
            data.orientation = Utilities::AssimpGlmHelper::GetQuat(aiOrientation);
            data.orientation.Normalize();

            data.timeStamp = timeStamp;
            rotations.push_back(data);
        }

        numScalings = channel->mNumScalingKeys;
        for (int keyIndex = 0; keyIndex < numScalings; ++keyIndex)
        {
            aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
            float timeStamp = channel->mScalingKeys[keyIndex].mTime;
            KeyScale data;
            data.scale = Utilities::AssimpGlmHelper::GetGLMVec(scale);
            data.timeStamp = timeStamp;
            scales.push_back(data);
        }
    }

    /// <summary>
    /// Update Bone transformation(VQS)
    /// </summary>
    /// <param name="animationTime">: Elapsed time to determine which keyframes will be used to interpolate</param>
    void Bone::Update(float animationTime)
    {
        localVQS = InterpolatingVQS(animationTime);
    }

    // Gets the current index on mKeyPositions to interpolate to based on
    // the current animation time 
    int Bone::GetPositionIndex(float animationTime)
    {
        for (int index = 0; index < numPositions - 1; ++index)
        {
            if (animationTime < positions[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }

    // Gets the current index on mKeyRotations to interpolate to based on the
    // current animation time
    int Bone::GetRotationIndex(float animationTime)
    {
        for (int index = 0; index < numRotations - 1; ++index)
        {
            if (animationTime < rotations[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }

    // Gets the current index on mKeyScalings to interpolate to based on the
    // current animation time 
    int Bone::GetScaleIndex(float animationTime)
    {
        for (int index = 0; index < numScalings - 1; ++index)
        {
            if (animationTime < scales[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }

    /// <summary>
    /// Get 't' for interpolating.
    /// <para>while t: [0:1]</para>
    /// </summary>
    /// <param name="lastTimeStamp">p0: previous keyframe</param>
    /// <param name="nextTimeStamp">p1: next keyframe</param>
    /// <param name="animationTime">: Elapsed time to determine which keyframes will be used to interpolate</param>
    /// <returns></returns>
    float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        float scaleFactor = 0.0f;

        float midWayLength = animationTime - lastTimeStamp;

        float framesDiff = nextTimeStamp - lastTimeStamp;

        scaleFactor = midWayLength / framesDiff;

        return scaleFactor;
    }

    // figures out which position keys to interpolate b/w and performs the interpolation
    // and returns the translation matrix
    glm::mat4 Bone::InterpolatePosition(float animationTime)
    {
        if (numPositions == 1)
            return glm::translate(glm::mat4(1.0f), positions[0].position);

        int p0Index = GetPositionIndex(animationTime);
        int p1Index = p0Index + 1;

        float scaleFactor = GetScaleFactor(positions[p0Index].timeStamp,
            positions[p1Index].timeStamp, animationTime);

        glm::vec3 finalPosition = Math::Lerp(positions[p0Index].position, positions[p1Index].position, scaleFactor);

        return glm::translate(glm::mat4(1.0f), finalPosition);
    }

    // figures out which rotations keys to interpolate b/w and performs the interpolation
    // and returns the rotation matrix
    glm::mat4 Bone::InterpolateRotation(float animationTime)
    {
        if (1 == numRotations)
        {
            return rotations[0].orientation.GetRotationMatrix();
        }

        int p0Index = GetRotationIndex(animationTime);
        int p1Index = p0Index + 1;

        float scaleFactor = GetScaleFactor(rotations[p0Index].timeStamp,
            rotations[p1Index].timeStamp, animationTime);

        Math::Quaternion finalRotation = Math::Slerp(rotations[p0Index].orientation,
                                                    rotations[p1Index].orientation, 
                                                    scaleFactor);

        return finalRotation.GetRotationMatrix();
    }

    // figures out which scaling keys to interpolate b/w and performs the interpolation
    // and returns the scale matrix
    glm::mat4 Bone::InterpolateScaling(float animationTime)
    {
        if (1 == numScalings)
            return glm::scale(glm::mat4(1.0f), scales[0].scale);

        int p0Index = GetScaleIndex(animationTime);
        int p1Index = p0Index + 1;

        float scaleFactor = GetScaleFactor(scales[p0Index].timeStamp,
            scales[p1Index].timeStamp, animationTime);

        // only take y scale to use elerp
        glm::vec3 finalScale = glm::vec3(Math::Elerp(scales[p0Index].scale.y, scales[p1Index].scale.y, scaleFactor));

        return glm::scale(glm::mat4(1.0f), finalScale);
    }

    /// <summary>
    /// Interpolate VQS of each Bone
    /// </summary>
    /// <param name="animationTime">: Elapsed time to determine which keyframes will be used to interpolate</param>
    /// <returns></returns>
    Math::VQS Bone::InterpolatingVQS(float animationTime)
    {

        Math::VQS iFrameVQS;

        int p0Index = GetPositionIndex(animationTime);
        int p1Index = p0Index + 1;

        float scaleFactorTranslate = GetScaleFactor(positions[p0Index].timeStamp,
                                                    positions[p1Index].timeStamp, animationTime);

        /// Lerp VQS translation Vector ///
        iFrameVQS.v = Math::Lerp(positions[p0Index].position, 
                                 positions[p1Index].position, 
                                 scaleFactorTranslate);

        p0Index = GetRotationIndex(animationTime);
        p1Index = p0Index + 1;

        float scaleFactorRotation = GetScaleFactor(rotations[p0Index].timeStamp,
                                                   rotations[p1Index].timeStamp, animationTime);

        /// SLerp VQS rotation Quaternion ///
        iFrameVQS.q = Math::Slerp(rotations[p0Index].orientation,
                                  rotations[p1Index].orientation,
                                  scaleFactorRotation);

        p0Index = GetScaleIndex(animationTime);
        p1Index = p0Index + 1;

        float scaleFactorScale = GetScaleFactor(scales[p0Index].timeStamp,
                                                scales[p1Index].timeStamp, animationTime);

        /// ELerp scale float ///
        /// as VQS only support uniform scaling, use only y in this case
        iFrameVQS.s = Math::Elerp(scales[p0Index].scale.y, 
                                  scales[p1Index].scale.y, 
                                  scaleFactorScale);

        return iFrameVQS;
    }
}