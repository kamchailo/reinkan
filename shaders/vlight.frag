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

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 vertexNormal;

layout(location = 0) out vec4 outColor;

void main()
{
    vec4 shaftColor = pushConstant.lightConstant;
    float shaftDistance = length(pushConstant.cameraPosition.xyz - worldPos) * 0.1;
    // float shaftDistance = gl_FragCoord.z;

    // shaftColor = vec4(0,1,0,1);

    int facing = 1;

    if(gl_FrontFacing)
    {
        facing = -1;
        // outColor = vec4(1,0,0,1);
        // return;
    }

    // outColor = vec4(0,1,0,1);
    // return;

    outColor = facing * shaftDistance * shaftColor;
    // outColor = vec4(vec3(shaftDistance), 1.0);
}