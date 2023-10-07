#include "pch.h"
#include "Quaternion.h"

namespace  Reinkan::Math
{
	void Quaternion::Normalize()
	{
		*this /= this->Length(*this);
	}

	Quaternion Quaternion::GetInverse() const
	{
		//return Quaternion(s, -v) / LengthSquared(*this);
		return Quaternion(s, -v);
	}

	/// <summary>
	/// Get Matrix form of the quaternion for rotation
	/// </summary>
	/// <returns>Rotation Matrix</returns>
	glm::mat4 Quaternion::GetRotationMatrix() const
	{
		glm::mat4 rotationMatrix;

		float x = v[0];
		float y = v[1];
		float z = v[2];

		// 1st Column
		rotationMatrix[0][0] = 1 - 2 * (y * y + z * z);
		rotationMatrix[0][1] = 2 * (x * y + s * z);
		rotationMatrix[0][2] = 2 * (x * z - s * y);
		rotationMatrix[0][3] = 0.0f;

		// 2nd Column
		rotationMatrix[1][0] = 2 * (x * y - s * z);
		rotationMatrix[1][1] = 1 - 2 * (x * x + z * z);
		rotationMatrix[1][2] = 2 * (y * z + s * x);
		rotationMatrix[1][3] = 0.0f;

		// 3rd Column
		rotationMatrix[2][0] = 2 * (x * z + s * y);
		rotationMatrix[2][1] = 2 * (y * z - s * x);
		rotationMatrix[2][2] = 1 - 2 * (x * x + y * y);
		rotationMatrix[2][3] = 0.0f;

		// 4th Column
		rotationMatrix[3][0] = 0.0f;
		rotationMatrix[3][1] = 0.0f;
		rotationMatrix[3][2] = 0.0f;
		rotationMatrix[3][3] = 1.0f;

		return rotationMatrix;
	}

	Quaternion& Quaternion::operator=(Quaternion const& q)
	{
		this->s = q.s;
		this->v = q.v;
		return *this;
	}

	Quaternion Quaternion::operator-() const
	{
		Quaternion result(*this);
		result.s = -result.s;
		result.v = -result.v;

		return result;
	}

	Quaternion Quaternion::operator+(Quaternion const& op2) const
	{
		Quaternion result(*this);

		return result += op2;
	}

	Quaternion Quaternion::operator-(Quaternion const& op2) const
	{
		Quaternion result(*this);

		return result -= op2;
	}

	/// <summary>
	/// Multiply between Quaternion
	/// <para>Also concatenation</para>
	/// </summary>
	/// <param name="op2">: Another quaternion that will get transform by this quaternion</param>
	/// <returns>Transformed quaternion</returns>
	Quaternion Quaternion::operator*(Quaternion const& op2) const
	{
		Quaternion result(*this);

		return result *= op2;
	}

	Quaternion Quaternion::operator/(Quaternion const& op2) const
	{
		Quaternion result(*this);

		return result /= op2;
	}

	Quaternion Quaternion::operator+=(Quaternion const& rhs)
	{
		this->s += rhs.s;
		this->v.x += rhs.v.x;
		this->v.y += rhs.v.y;
		this->v.z += rhs.v.z;

		return *this;
	}

	Quaternion Quaternion::operator-=(Quaternion const& rhs)
	{
		this->s   -= rhs.s;
		this->v.x -= rhs.v.x;
		this->v.y -= rhs.v.y;
		this->v.z -= rhs.v.z;

		return *this;
	}

	/// <summary>
	/// Multiply between Quaternion
	/// <para>Also concatenation</para>
	/// </summary>
	/// <param name="rhs">: Another quaternion that will get transform by this quaternion</param>
	/// <returns>Transformed quaternion</returns>
	Quaternion Quaternion::operator*=(Quaternion const& rhs)
	{
		float tempS = this->s;
		this->s = this->s * rhs.s - glm::dot(this->v, rhs.v);

		this->v = tempS * rhs.v +
				  rhs.s   * this->v +
				  glm::cross(this->v, rhs.v);

		return *this;
	}

	Quaternion Quaternion::operator/=(Quaternion const& rhs)
	{
		Quaternion inverseRhs(rhs.GetInverse());

		return *this * inverseRhs;
	}
	
	Quaternion Quaternion::operator*(float const& scalar) const
	{
		Quaternion result(*this);

		return result *= scalar;
	}

	Quaternion Quaternion::operator/(float const& denom) const
	{
		Quaternion result(*this);

		return result /= denom;
	}

	Quaternion Quaternion::operator*=(float const& scalar)
	{
		this->s *= scalar;
		this->v *= scalar;

		return *this;
	}

	Quaternion Quaternion::operator/=(float const& denom)
	{
		this->s /= denom;
		this->v /= denom;

		return *this;
	}

	/// <summary>
	/// Transform  vector using this Quaternion
	/// </summary>
	/// <param name="vector">origin orientation</param>
	/// <returns>transformed orientation</returns>
	glm::vec3 Quaternion::operator*(glm::vec3 const& vector) const
	{
		Quaternion inverse = GetInverse();
		Quaternion pureQuaternion = Quaternion(0, vector);

		return (*this * pureQuaternion * inverse).v;
	}
}
