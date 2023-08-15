#version 450

struct PushConstant
{
    mat4 modelMatrix;
	uint objectId;
    uint materialId;
};
layout(push_constant) uniform PushConstantRaster_T
{
    PushConstant pushConstant;
};

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 normal;
layout(location = 1) out vec2 fragTexCoord;

void main() 
{
    gl_Position = ubo.proj * ubo.view * pushConstant.modelMatrix *  vec4(inPosition, 1.0);
    normal = inColor;
    fragTexCoord = inTexCoord;
}