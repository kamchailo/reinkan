#pragma once

#include "Math/Quaternion.h"
#include "Math/Interpolation.hpp"

namespace Reinkan::Math
{
	class VQS
	{
	public:
		
		glm::vec3 v{ 0 };

		Math::Quaternion q{};

		float s{ 1 };

		VQS() {};

		VQS(glm::vec3 v, Math::Quaternion q, float s) : v(v), q(q), s(s) {};

		glm::mat4 ToMatrix() const;

		void Normalize()
		{
			q.Normalize();
		}

		void Print() const;

		VQS operator* (VQS const& op2) const;

		VQS operator*= (VQS const& rhs);

		glm::vec3 operator* (glm::vec3 const& op2) const;
	};
}