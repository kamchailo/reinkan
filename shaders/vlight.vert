#version 450

#include "SharedStruct.glsl"

layout(push_constant) uniform PushConstantVLight_T
{
    PushConstantVLight pushConstant;
};

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 model;
    mat4 view;
    mat4 viewInverse;
    mat4 proj;
    mat4 shadowProjectionViewMatrix;
    vec3 globalLightPosition;
    uint globalLightPosition_padding;
    vec2 screenExtent;
} ubo;

layout(binding = 1) uniform sampler2D shadowmap;

layout(location = 0) in vec3    inPosition;
layout(location = 1) in vec3    inVertexNormal;

layout(location = 0) out vec3 worldPos;
layout(location = 1) out vec3 vertexNormal;

void main()
{
    // no model matrix needed as pyramid will be in world position
    mat4 modelTransform = ubo.proj * ubo.view;

    vec2 shadowIndex;
    shadowIndex.y = gl_VertexIndex / ubo.screenExtent.x;
    shadowIndex.x = gl_VertexIndex % int(ubo.screenExtent.x);

    // out
    worldPos = texture(shadowmap, shadowIndex).xyz;
    vertexNormal = vec3(0, 1, 0);

    gl_Position = vec4(worldPos, 1);
}