#version 450

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 viewDir;

layout(location = 0) out vec4 outColor;

void main() {

    // vec2 coord = gl_PointCoord - vec2(0.5);
    // outColor = vec4(fragColor, 0.5 - length(coord));
    outColor = vec4(worldPos, 1.0);
}