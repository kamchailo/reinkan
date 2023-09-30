#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
// Change to Engine Quaternion
#include <glm/gtc/quaternion.hpp>

namespace Reinkan::Graphics::Animation
{
    struct KeyPosition
    {
        glm::vec3 position;
        float timeStamp;
    };

    struct KeyRotation
    {
        glm::quat orientation;
        float timeStamp;
    };

    struct KeyScale
    {
        glm::vec3 scale;
        float timeStamp;
    };

    class Bone
    {
    private:
        std::vector<KeyPosition> positions;
        std::vector<KeyRotation> rotations;
        std::vector<KeyScale> scales;
        int numPositions;
        int numRotations;
        int numScalings;

        glm::mat4 localTransform;
        std::string name;
        int id;
    };
}