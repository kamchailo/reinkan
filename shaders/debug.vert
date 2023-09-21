#version 450

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 model;
    mat4 view;
    mat4 viewInverse;
    mat4 proj;
    uint lightNumber;
    vec2 screenExtent;
} ubo;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 worldPos;
layout(location = 1) out vec3 viewDir;

void main() 
{
    // gl_Position =  ubo.proj * ubo.view * ubo.model * pushConstant.modelMatrix *  vec4(inPosition, 1.0);
    // mat4 modelTransform = ubo.proj * ubo.view * pushConstant.modelMatrix;
    mat4 modelTransform = ubo.proj * ubo.view * ubo.model;

    gl_Position =  modelTransform * vec4(inPosition, 1.0);
    
    vec3 eye = vec3(ubo.viewInverse * vec4(0, 0, 0, 1));

    // out
    worldPos = gl_Position.xyz;
    viewDir = vec3(eye - worldPos);
}