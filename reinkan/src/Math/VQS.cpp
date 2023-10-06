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
        glm::vec3 sr = s * r;

        return q * sr + v;
    }
}

