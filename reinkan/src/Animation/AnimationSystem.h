#pragma once

#include <map>
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "Animation.h"
#include "Animation/Animator.h"
#include "Structures/BoneInfo.h"

namespace Reinkan::Animation
{
	class AnimationSystem
	{
	public:

		void Init();

		void Update();

		void Shutdown();

		void AddBoneName(std::string boneName);

		void SetBoneOffset(std::string boneName, glm::mat4 offset);

		int32_t GetBoneId(std::string boneName);

		void LoadAnimation(std::string animationPath);

		std::map<std::string, BoneInfo>& GetMapBoneName();

		void AddAnimation(Animation animation);

		void AddAnimator(Animator animator);

		Animation* GetCurrentAnimation() const;
		
		Animator* GetcurrentAnimator() const;

	private:

		std::vector<Animation> animations;

		std::vector<Animator> animators;

		std::map<std::string, BoneInfo> mapBoneName;

		Animation* currentAnimation;

		Animator* currentAnimator;

		uint32_t boneCount{ 0 };
	};
}