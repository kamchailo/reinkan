#version 450

#extension GL_EXT_nonuniform_qualifier : enable
// #extension GL_EXT_buffer_reference2 : require

struct PushConstant
{
    mat4 modelMatrix;
	int objectId;
    int materialId;
};
layout(push_constant) uniform PushConstantRaster_T
{
    PushConstant pushConstant;
};

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 model;
    mat4 view;
    mat4 viewInverse;
    mat4 proj;
    uint lightNumber;
} ubo;

struct Material
{    
    vec3 diffuse;
    uint diffuse_padding;
    vec3 specular;
    uint specular_padding;
    uint normalMapId;
    uint heightMapId;
    float shininess;
    uint diffuseMapId;
};
layout(binding = 1) buffer MaterialBlock 
{
    // Get material by using PushConstant::materialId
    // Material material = materials[pushConstant.materialId];
    Material materials[];
};

layout(binding = 2) uniform sampler2D[] textureSamplers;
// pack all textures together (diffuse, normal, height)
// Unpack by using Material::**texture**Id to Offset
// vec3 diffuse = textureSamplers[material.diffuseMapId];
// vec3 normal = textureSamplers[material.normalMapId];
// vec3 height = textureSamplers[material.heightMapId];

struct LightObject
{
    vec3	position;
    uint    position_padding;
    vec3	color;
    uint    color_padding;
    float	intensity;
    float   radius;
};
layout(std140, binding = 3) readonly buffer GlobalLightSSBO {
   LightObject globalLights[ ];
};  

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexTangent;
layout(location = 3) in vec3 viewDir;
layout(location = 4) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

// include BRDF calculation
#include "brdf.glsl"

void main() 
{
    Material material = materials[pushConstant.materialId];
    
    vec3 diffuse = texture(textureSamplers[material.diffuseMapId], fragTexCoord).rgb;

    // overrride material
    material.diffuse = diffuse;

    vec3 N = normalize(vertexNormal);
    vec3 normalMap = texture(textureSamplers[material.normalMapId], fragTexCoord).rgb;
    if(material.normalMapId <= 200)
    {
        // Need to calculate vertexNormal while loading model
        // N = normalize(normalMap * 2.0 - 1.0);
    }


    vec3 L = normalize(vec3(1.0, 3.0, 1.0) - worldPos);
    float ambientLight = 0.4;
    float intensity = 0.7;
    vec3 V = normalize(viewDir);
    // vec3 worldPosVisible = worldPos * 10

    vec3 brdfColor = intensity * EvalBrdf(N, L, V, material);

    for(int i = 0; i < ubo.lightNumber; ++i)
    {
        LightObject light = globalLights[i];
        float lightDistance = distance(light.position, worldPos);
        if(lightDistance >= light.radius)
        {
            continue;
        }
        L = normalize(light.position - worldPos);
        float intensity = light.intensity * (light.radius - lightDistance) / light.radius;
        brdfColor += light.intensity * light.color * EvalBrdf(N, L, V, material);
    }
  
    outColor = vec4(brdfColor, 1.0);
/*

    if(ubo.lightNumber > 0)
    {
        outColor = vec4(lightDistance, lightDistance, lightDistance, 1.0);
    }
*/    

    // float depth = length(viewDir) / 10.0;

    // outColor = vec4(vec3(depth), 1.0);
    // outColor = vec4(worldPos, 1.0);

    // outColor  = vec4(material.diffuse, 1.0);
}