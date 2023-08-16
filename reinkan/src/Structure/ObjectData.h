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
        BufferWrap      vertexBufferWrap;   // Buffer of vertices 
        BufferWrap      indexBufferWrap;    // Buffer of triangle indices
        uint32_t        objectId;
        uint32_t        materialId;
        //BufferWrap      matColorBuffer; // Buffer of materials
        //BufferWrap      matIndexBuffer; // Buffer of each triangle's material index
	};
}