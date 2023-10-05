#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Animation/Animation.h"

namespace Reinkan::Animation
{
    class Animator
    {
    public:

        Animator(Animation* Animation);

        void UpdateAnimation();

        void PlayAnimation(Animation* pAnimation);

        void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

        AnimationUniformBufferObject& GetFinalBoneMatrices();

        glm::vec3* GetDebugVertices();

    private:

        AnimationUniformBufferObject m_FinalBoneMatrices;

        Animation* m_CurrentAnimation;
        
        float m_CurrentTime;
        
        float m_DeltaTime;

        glm::vec3 debugVertices[MAX_BONE * 2];
    };

}