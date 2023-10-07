#pragma once

#include <glm/glm.hpp>
//#include <glm/gtx/quaternion.hpp>
#include "Math/Quaternion.h"
namespace Reinkan::Animation
{

    /// <summary>
    /// Struct for storing position value in keyframe
    /// </summary>
    struct KeyPosition
    {
        glm::vec3 position;
        float timeStamp;
    };

    /// <summary>
    /// Struct for storing orientation value in keyframe
    /// </summary>
    struct KeyRotation
    {
        Math::Quaternion orientation;
        float timeStamp;
    };

    /// <summary>
    /// Struct for storing scale value in keyframe
    /// </summary>
    struct KeyScale
    {
        glm::vec3 scale;
        float timeStamp;
    };

}