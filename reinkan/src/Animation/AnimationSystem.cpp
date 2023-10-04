#include "pch.h"
#include "AnimationSystem.h"

namespace Reinkan::Animation
{
	void AnimationSystem::Init()
	{

		Animation animation("../assets/models/Walking.dae");
		AddAnimation(animation);

		Animator animator(currentAnimation);
		AddAnimator(animator);
		
		currentAnimator->PlayAnimation(currentAnimation);
	}

	void AnimationSystem::Update()
	{
		for (auto& animator : animators)
		{
			animator.UpdateAnimation();
		}
	}

	void AnimationSystem::Shutdown()
	{
	}

	void AnimationSystem::AddBoneName(std::string boneName)
	{
		if (mapBoneName.find(boneName) != mapBoneName.end())
		{
			return;
		}

		mapBoneName[boneName].id = boneCount;

		++boneCount;
	}

	void AnimationSystem::SetBoneOffset(std::string boneName, glm::mat4 offset)
	{
		if (mapBoneName.find(boneName) == mapBoneName.end())
		{
			std::printf("Can not set bone offset: Bone not found.");
			return;
		}

		mapBoneName[boneName].offset = offset;
	}

	int32_t AnimationSystem::GetBoneId(std::string boneName)
	{
		if (mapBoneName.find(boneName) == mapBoneName.end())
		{
			return -1;
		}

		return mapBoneName[boneName].id;
	}

	std::map<std::string, BoneInfo>& AnimationSystem::GetMapBoneName()
	{
		return mapBoneName;
	}

	void AnimationSystem::AddAnimation(Animation animation)
	{
		animations.push_back(animation);
		currentAnimation = &animations.back();
	}

	void AnimationSystem::AddAnimator(Animator animator)
	{	
		animators.push_back(animator);
		currentAnimator = &animators.back();;
	}

	Animation* AnimationSystem::GetCurrentAnimation() const
	{
		return currentAnimation;
	}
	Animator* AnimationSystem::GetcurrentAnimator() const
	{
		return currentAnimator;
	}
}

