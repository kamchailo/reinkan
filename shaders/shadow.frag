#version 450

struct PushConstantShadow
{
    mat4 modelMatrix;
    vec2 screenExtent;
};
layout(push_constant) uniform PushConstantShadow_T
{
    PushConstantShadow pushConstant;
};

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 viewDir;

layout(location = 0) out float outColor;

void main()
{
    outColor = viewDir.z;
}