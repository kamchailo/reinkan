#include "pch.h"
#include "Animator.h"

#include <array>

#include "Core/Locator/TimeSystemLocator.h"
#include "../shaders/SharedStruct.h"

namespace Reinkan::Animation
{
	Animator::Animator(Animation* animation)
    {
        m_CurrentTime = 0.0;
        m_CurrentAnimation = animation;

        for (int i = 0; i < MAX_BONE; ++i)
        {
            m_FinalBoneMatrices.boneMatrices[i] = glm::mat4(1.0f);
            debugVertices[i * 2] = glm::vec3(0);
            debugVertices[i * 2 + 1] = glm::vec3(0);
        }

    }

    void Animator::UpdateAnimation()
    {
        m_DeltaTime = Core::TimeSystemLocator::GetTime()->GetDeltaTime();
        if (m_CurrentAnimation)
        {
            m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * m_DeltaTime;
            m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
            
            // CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));

            // positioning the root by change the initial VQS
            CalculateBoneVQS(&m_CurrentAnimation->GetRootNode(), Math::VQS());
        }
    }

    void Animator::PlayAnimation(Animation* pAnimation)
    {
        m_CurrentAnimation = pAnimation;
        m_CurrentTime = 0.0f;
    }             

    void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
    {
        std::string nodeName = node->name;    
        glm::mat4 nodeTransform = node->transformation;

        Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

        if (Bone)
        {
            Bone->Update(m_CurrentTime);        
            nodeTransform = Bone->GetLocalTransform();
        }
    
        glm::mat4 globalTransformation = parentTransform * nodeTransform;

        auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            int index = boneInfoMap[nodeName].id;
            glm::mat4 offset = boneInfoMap[nodeName].offset;
        
            /// Set Final Matrix in Shader ///
            m_FinalBoneMatrices.boneMatrices[index] = { globalTransformation * offset };

            debugVertices[index * 2] = globalTransformation[3];

            if (node->parent)
            {
                debugVertices[index * 2 + 1] = parentTransform[3];
            }
            else
            {
                debugVertices[index * 2 + 1] = globalTransformation[3];
            }
        }

        for (int i = 0; i < node->childrenCount; i++)
        {
            CalculateBoneTransform(&node->children[i], globalTransformation);
        }
    }

    void Animator::CalculateBoneVQS(const AssimpNodeData* node, Math::VQS parentVQS)
    {
        std::string nodeName = node->name;        
        Math::VQS nodeVQS = node->vqs;

        Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

        if (Bone)
        {
            Bone->Update(m_CurrentTime);            
            nodeVQS = Bone->GetLocalVQS();
        }
        
        /// Perform VQS Concatenation ///
        Math::VQS globalVQS = parentVQS * nodeVQS;

        auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            int index = boneInfoMap[nodeName].id;
            glm::mat4 offset = boneInfoMap[nodeName].offset;

            /// Set Final Matrix in Shader ///
            m_FinalBoneMatrices.boneMatrices[index] = { globalVQS.ToMatrix() * offset };

            debugVertices[index * 2] = globalVQS.v;

            if (node->parent)
            {
                // TODO: check if need to convert to matrix first, 
                // parent scale should affect translation
                debugVertices[index * 2 + 1] = parentVQS.v;
            }
            else
            {
                debugVertices[index * 2 + 1] = globalVQS.v;
            }
        }

        for (int i = 0; i < node->childrenCount; i++)
        {
            CalculateBoneVQS(&node->children[i], globalVQS);
        }
    }

    AnimationUniformBufferObject& Animator::GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

    glm::vec3* Animator::GetDebugVertices()
    {
        return debugVertices;
    }
}