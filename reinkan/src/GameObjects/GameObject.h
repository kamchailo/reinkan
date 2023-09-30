#pragma once

#include <glm/glm.hpp>

namespace Reinkan::GameObject
{
	class GameObject
	{
	public:
		GameObject();

		~GameObject();

		void Init();

		void Update();

		void Destroy();

	private:
		bool isDestroyed;

		glm::mat4 transformMatrix;
			 
	};
}