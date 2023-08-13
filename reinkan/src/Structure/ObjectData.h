#pragma once

#include <glm/glm.hpp>

#include "Structure/BufferWrap.h"

namespace Reinkan
{
	struct ObjectData
	{
		uint32_t        nbIndices{0};
        uint32_t        nbVertices{0};
        glm::mat4       transform;      // Instance matrix of the object
        BufferWrap      vertexBuffer;   // Buffer of vertices 
        BufferWrap      indexBuffer;    // Buffer of triangle indices
        //BufferWrap      matColorBuffer; // Buffer of materials
        //BufferWrap      matIndexBuffer; // Buffer of each triangle's material index
	};
}