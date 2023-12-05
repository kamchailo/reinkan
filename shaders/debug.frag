#version 450

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 viewDir;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outPosition;

void main() {

    // vec2 coord = gl_PointCoord - vec2(0.5);
    // outColor = vec4(fragColor, 0.5 - length(coord));
    vec3 color = worldPos * 500.0;
    outColor = vec4(worldPos, 1.0);
    outPosition = vec4(worldPos, 1.0);
}