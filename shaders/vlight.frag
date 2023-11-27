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
layout(binding = 2) uniform sampler2D depthmap;

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 vertexNormal;

layout(location = 0) out vec4 outColor;

void main()
{
    vec4 shaftColor = pushConstant.lightConstant;
    // float shaftDistance = length(pushConstant.cameraPosition.xyz - worldPos);
    float shaftDistance = gl_FragCoord.z;
    
    float bias = 0.1;
    float sceneDepth =  texture(depthmap, gl_FragCoord.xy).r + bias;
    shaftDistance = min(shaftDistance, sceneDepth);

    int facing = 1;

    if(gl_FrontFacing)
    {
        facing = -1;
    }

    float scale = 0.5;
    outColor = facing * shaftDistance * shaftColor * scale;
    // outColor = vec4(vec3(shaftDistance), 1.0);

}