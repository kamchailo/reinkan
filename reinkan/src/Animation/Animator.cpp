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
            debugVertices[i].first = glm::vec3(0);
            debugVertices[i].second = glm::vec3(0);
        }

    }

    void Animator::UpdateAnimation()
    {
        m_DeltaTime = Core::TimeSystemLocator::GetTime()->GetDeltaTime();
        if (m_CurrentAnimation)
        {
            m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * m_DeltaTime;
            m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
            
            // TODO:
            // positioning by change the matrix pass here
            CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
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
            m_FinalBoneMatrices.boneMatrices[index] = { globalTransformation * offset };

            debugVertices[index].first = glm::vec3((globalTransformation * offset * glm::vec4(0, 0, 0, 1)));
            debugVertices[index].second = glm::vec3((parentTransform * offset * glm::vec4(0, 0, 0, 1)));

            /*
            std::printf("Write at index %d : value\n", index);
            std::printf("%f %f %f %f\n", m_FinalBoneMatrices.boneMatrices[index][0][0], m_FinalBoneMatrices.boneMatrices[index][1][0], m_FinalBoneMatrices.boneMatrices[index][2][0], m_FinalBoneMatrices.boneMatrices[index][3][0]);
            std::printf("%f %f %f %f\n", m_FinalBoneMatrices.boneMatrices[index][0][1], m_FinalBoneMatrices.boneMatrices[index][1][1], m_FinalBoneMatrices.boneMatrices[index][2][1], m_FinalBoneMatrices.boneMatrices[index][3][1]);
            std::printf("%f %f %f %f\n", m_FinalBoneMatrices.boneMatrices[index][0][2], m_FinalBoneMatrices.boneMatrices[index][1][2], m_FinalBoneMatrices.boneMatrices[index][2][2], m_FinalBoneMatrices.boneMatrices[index][3][2]);
            std::printf("%f %f %f %f\n", m_FinalBoneMatrices.boneMatrices[index][0][3], m_FinalBoneMatrices.boneMatrices[index][1][3], m_FinalBoneMatrices.boneMatrices[index][2][3], m_FinalBoneMatrices.boneMatrices[index][3][3]);

            std::printf("Bone #%d, {%f, %f, %f} {%f, %f, %f}\n", index,
                debugVertices[index].first.x, debugVertices[index].first.y, debugVertices[index].first.z,
                debugVertices[index].second.x, debugVertices[index].second.y, debugVertices[index].second.z);
            */
        }

        for (int i = 0; i < node->childrenCount; i++)
        {
            CalculateBoneTransform(&node->children[i], globalTransformation);
        }
    }

    AnimationUniformBufferObject& Animator::GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

    std::pair<glm::vec3, glm::vec3>* Animator::GetDebugVertices()
    {
        return debugVertices;
    }
}