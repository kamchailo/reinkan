#pragma once

#include <map>
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "Animation.h"
#include "Structures/BoneInfo.h"

namespace Reinkan::Animation
{
	class AnimationSystem
	{
	public:

		void Init();

		void Update();

		void AddBoneName(std::string boneName);

		void SetBoneOffset(std::string boneName, glm::mat4 offset);

		int32_t GetBoneId(std::string boneName);

		void LoadAnimation(std::string animationPath);

		std::map<std::string, BoneInfo>& GetMapBoneName();

		void AddAnimation(Animation animation);



		std::vector<AnimationUniformBufferObject> animationMatrices;

	private:

		std::vector<Animation> animations;

		std::map<std::string, BoneInfo> mapBoneName;

		uint32_t boneCount{ 0 };
	};
}