#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Animation/Animation.h"
#include "Math/VQS.h"

namespace Reinkan::Animation
{
    class Animator
    {
    public:

        Animator(Animation* Animation);

        void UpdateAnimation();

        void PlayAnimation(Animation* pAnimation);

        void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

        void CalculateBoneVQS(const AssimpNodeData* node, Math::VQS parentVQS);

        AnimationUniformBufferObject& GetFinalBoneMatrices();

        glm::vec3* GetDebugVertices();

    private:

        AnimationUniformBufferObject m_FinalBoneMatrices;

        Animation* m_CurrentAnimation{ nullptr };
        
        float m_CurrentTime{ 0 };
        
        float m_DeltaTime{ 0 };

        glm::vec3 debugVertices[MAX_BONE * 2];
    };

}