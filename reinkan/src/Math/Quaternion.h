#pragma once

#include <glm/glm.hpp>

namespace Reinkan::Math
{
	class Quaternion
	{
	public:

		float s;
		glm::vec3 v;

		Quaternion() : s(1), v(0) {};
		Quaternion(float s, glm::vec3 v) : s{ s }, v{ v } {};
		Quaternion(float s, float x, float y, float z) : s{ s }, v( x, y, z ) {};

		void Normalize();

		glm::mat4 ToMatrix() const;

		glm::mat4 GetRotationMatrix() const;

		Quaternion& operator=(Quaternion const& q);

		// Unary negative
		Quaternion operator-() const;

		Quaternion operator+ (Quaternion const& op2) const;
		Quaternion operator- (Quaternion const& op2) const;
		Quaternion operator* (Quaternion const& op2) const;
		Quaternion operator/ (Quaternion const& op2) const;

		Quaternion operator+= (Quaternion const& rhs);
		Quaternion operator-= (Quaternion const& rhs);
		Quaternion operator*= (Quaternion const& rhs);
		Quaternion operator/= (Quaternion const& rhs);

		// Arithmetic Operatoions
		Quaternion operator*(float const& scalar) const;
		Quaternion operator/(float const& denom) const;

		Quaternion operator*=(float const& scalar);
		Quaternion operator/=(float const& denom);

		static inline float Dot(Quaternion const& q1, Quaternion const& q2) 
		{
			return (q1.s * q2.s + 
					q1.v.x * q2.v.x + 
					q1.v.y * q2.v.y + 
					q1.v.z * q2.v.z );
		}

		static inline float Angle(Quaternion const& q1, Quaternion const& q2) 
		{
			Quaternion q3{ q2 };
			float dot = Dot(q1, q2);

			if (dot < 0)
			{
				dot = -dot;
				q3 = -q2;
			}

			return std::acosf(dot);
		}

		static inline float Length(Quaternion const& q) 
		{
			float length = q.s * q.s + 
						   q.v.x * q.v.x + 
						   q.v.y * q.v.y + 
						   q.v.z * q.v.z;

			return std::sqrt(length);
		}

		static inline float LengthSquared(Quaternion const& q) 
		{
			float length = q.s * q.s + 
						   q.v.x * q.v.x + 
						   q.v.y * q.v.y + 
						   q.v.z * q.v.z;

			return length;
		}

		static Quaternion Unit(Quaternion const& q) {
			return q * (1.0f / Quaternion::Length(q));
		}
	};
}