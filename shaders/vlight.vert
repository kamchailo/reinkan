#version 450

#include "SharedStruct.glsl"

layout(push_constant) uniform PushConstantVLight_T
{
    PushConstantVLight pushConstant;
};

layout(binding = 0) uniform UniformBufferObject_T 
{
    UniformBufferObject ubo;
};

layout(binding = 1) uniform sampler2D shadowmap;

layout(location = 0) in vec3    inPosition;
layout(location = 1) in vec3    inVertexNormal;

layout(location = 0) out vec3 worldPos;
layout(location = 1) out vec3 vertexNormal;

void main()
{
    // no model matrix needed as pyramid will be in world position
    mat4 modelTransform = ubo.proj * ubo.view * mat4(1);

    vec2 shadowIndex;
    shadowIndex.y = floor(gl_VertexIndex / pushConstant.shadowMapExtent.x);
    shadowIndex.x = gl_VertexIndex % int(pushConstant.shadowMapExtent.x);

    shadowIndex.x = shadowIndex.x / pushConstant.shadowMapExtent.x;
    shadowIndex.y = shadowIndex.y / pushConstant.shadowMapExtent.y;

    vec4 positionFromShadowMap;

    if(gl_VertexIndex == pushConstant.shadowMapExtent.x * pushConstant.shadowMapExtent.y)
    {
        positionFromShadowMap = pushConstant.lightPosition; 
    }
    else
    {
        positionFromShadowMap =  texture(shadowmap, shadowIndex);
        positionFromShadowMap.w = 1.0;
    }

    vec4 basePosition = modelTransform * positionFromShadowMap;

    // out
    worldPos = positionFromShadowMap.xyz;
    vertexNormal = vec3(shadowIndex.x, shadowIndex.y, gl_VertexIndex);

    gl_Position = basePosition;
}