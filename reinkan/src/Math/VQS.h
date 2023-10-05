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

		void Normalize()
		{
			q.Normalize();
		}

		inline static VQS Interpolate(VQS const& a, VQS const& b, float t)
		{
			VQS result;

			// Lerp V
			result.v = Math::Lerp(a.v, b.v, t);

			// SLerp Q
			result.q = Math::Slerp(a.q, b.q, t);

			// ELerp S
			result.s = Math::Elerp(a.s, b.s, t);

			return result;
		}

		VQS operator* (VQS const& op2) const;

		VQS operator*= (VQS const& rhs);

		glm::vec3 operator* (glm::vec3 const& op2) const;
	};
}