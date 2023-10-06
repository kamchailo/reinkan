#include "pch.h"
#include "VQS.h"

namespace Reinkan::Math
{
    VQS VQS::operator*(VQS const& r) const
    {
        VQS vqs(*this);

        return vqs *= r;
    }

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
        return ((s * s - v * v) * r) +
                (2.0f * v * glm::dot(v, r)) +
                (2.0f * s * glm::cross(v, r)) + 
                v;
    }
}

