#include <stdint.h>

// Pair each instance with its instance transform
struct ObjInst
{
    glm::mat4 transform;    // Matrix of the instance
    uint32_t  objIndex;     // Model index
};

// Information of a obj model when referenced in a shader
struct ObjDesc
{
    int txtOffset;                 // Texture index offset in the array of textures
    uint64_t vertexAddress;        // Address of the Vertex buffer
    uint64_t indexAddress;         // Address of the index buffer
    uint64_t materialAddress;      // Address of the material buffer
    uint64_t materialIndexAddress; // Address of the triangle material index buffer
};

// The OBJ model: Vulkan buffers of object data
struct ObjData
{
    uint32_t nbIndices{0};
    uint32_t nbVertices{0};
    glm::mat4 transform;       // Instance matrix of the object
    BufferWrap vertexBuffer;   // Buffer of vertices
    BufferWrap indexBuffer;    // Buffer of triangle indices
    BufferWrap matColorBuffer; // Buffer of materials
    BufferWrap matIndexBuffer; // Buffer of each triangle's material index
};
