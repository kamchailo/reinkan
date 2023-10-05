#pragma once

#include <glm/glm.hpp>
//#include <glm/gtx/quaternion.hpp>
#include "Math/Quaternion.h"

namespace Reinkan::Animation
{
    struct KeyPosition
    {
        glm::vec3 position;
        float timeStamp;
    };

    struct KeyRotation
    {
        glm::quat orientationGLM;
        Math::Quaternion orientation;
        float timeStamp;
    };

    struct KeyScale
    {
        glm::vec3 scale;
        float timeStamp;
    };

}