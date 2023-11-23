#version 450

#include "SharedStruct.glsl"

layout(push_constant) uniform PushConstantRaster_T
{
    PushConstantPost pushConstant;
};

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D renderedImage;

void main()
{
    vec2 uv = gl_FragCoord.xy/pushConstant.screenExtent;
    // outColor = vec4(uv, 0, 1);
    outColor = vec4(texture(renderedImage,uv).rgb, 1);

    // outColor = vec4(uv, 0, 1);
}