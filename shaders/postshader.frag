#version 450

#include "SharedStruct.glsl"

layout(push_constant) uniform PushConstantRaster_T
{
    PushConstantPost pushConstant;
};

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D renderedImage;

layout(binding = 1) uniform sampler2D shadowMap;

layout(binding = 2) uniform sampler2D vlightMap;

void main()
{
    vec2 uv = gl_FragCoord.xy/pushConstant.screenExtent;
    // outColor = vec4(uv, 0, 1);
    outColor = vec4(texture(renderedImage, uv).rgb, 1);

    if((pushConstant.debugFlag & 0x2) > 1)
    {
        outColor = vec4(texture(shadowMap, uv).rgb, 1);
        // outColor = vec4(vec3(texture(shadowMap, uv).w) / 100, 1);
    }

    if((pushConstant.debugFlag & 0x4) > 1)
    {
        outColor = vec4(texture(vlightMap, uv).rgb, 1);
    }

    // outColor = vec4(uv, 0, 1);
}