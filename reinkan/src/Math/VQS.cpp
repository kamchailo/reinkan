#include "pch.h"
#include "VQS.h"

#include <glm/gtx/transform.hpp>

namespace Reinkan::Math
{
    /// <summary>
    /// Convert VQS to tranformation Matrix
    /// </summary>
    /// <returns>Tranformation Matrix</returns>
    glm::mat4 VQS::ToMatrix() const
    {
        // Add Diagonal as scalar
        glm::mat4 result = glm::mat4(1);

        //Scaling
        result[0][0] = s;
        result[1][1] = s;
        result[2][2] = s;

        result = q.GetRotationMatrix() * result;

        result[3] = glm::vec4(v, 1.0f);

        return result;
    }

    void VQS::Print() const
    {
        std::printf("======================================\n");
        std::printf("Vector:      \t <%f, %f, %f>\n", v.x, v.y, v.z);
        std::printf("Quaternion : \t <%f, %f, %f, %f>\n", q.s, q.v.x, q.v.y, q.v.z);
        std::printf("Scale:       \t %f\n", s);
        std::printf("======================================\n");
    }

    VQS VQS::operator*(VQS const& r) const
    {
        VQS vqs(*this);

        return vqs *= r;
    }

    /// <summary>
    /// Multiply between VQS
    /// <para>Also use for concatenation</para>
    /// </summary>
    /// <param name="rhs">: Another VQS that will get transform by this VQS</param>
    /// <returns>Transformed VQS</returns>
    VQS VQS::operator*=(VQS const& rhs)
    {
        // Vector Part
        this->v = *this * rhs.v;

        // Quaternion Part
        this->q *= rhs.q;

        // Scalar Part
        this->s *= rhs.s;

        return *this;
    }

    glm::vec3 VQS::operator*(glm::vec3 const& r) const
    {
        glm::vec3 sr = s * r;

        return q * sr + v;
    }
}

