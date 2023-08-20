#version 450

struct PushConstant
{
    mat4 modelMatrix;
	int objectId;
    int materialId;
};
layout(push_constant) uniform PushConstantRaster_T
{
    PushConstant pushConstant;
};

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 model;
    mat4 view;
    mat4 viewInverse;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inVertexNormal;
layout(location = 2) in vec3 inVertexTangent;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 worldPos;
layout(location = 1) out vec3 vertexNormal;
layout(location = 2) out vec3 vertexTangent;
layout(location = 3) out vec3 viewDir;
layout(location = 4) out vec2 fragTexCoord;

void main() 
{
    // gl_Position =  ubo.proj * ubo.view * ubo.model * pushConstant.modelMatrix *  vec4(inPosition, 1.0);
    gl_Position =  ubo.proj * ubo.view * pushConstant.modelMatrix * vec4(inPosition, 1.0);
    
    vec3 eye = vec3(ubo.viewInverse * vec4(0, 0, 0, 1));

    // out
    worldPos = vec3(pushConstant.modelMatrix * vec4(inPosition, 1.0));;
    vertexNormal = inVertexNormal;
    vertexTangent = inVertexTangent;
    viewDir = vec3(eye - worldPos);
    fragTexCoord = inTexCoord;
}