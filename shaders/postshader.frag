#version 450

#include "SharedStruct.glsl"

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D renderedImage;

void main()
{
    vec2 uv = gl_FragCoord.xy/vec2(1920, 1080);
    // outColor = vec4(uv, 0, 1);
    outColor = vec4(texture(renderedImage,uv).rgb, 1);
}