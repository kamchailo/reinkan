#version 450

#extension GL_EXT_nonuniform_qualifier : enable

// #extension GL_EXT_buffer_reference2 : require

// layout(binding = 1) uniform sampler2D texSampler;

struct PushConstant
{
    mat4 modelMatrix;
	uint objectId;
    uint materialId;
};
layout(push_constant) uniform PushConstantRaster_T
{
    PushConstant pushConstant;
};

struct Material
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
    uint diffuseMapId;
    uint normalMapId;
    uint heightMapId;
};
layout(binding = 1) buffer MaterialBlock 
{
    Material materials[];
};
// layout(buffer_reference, scalar) buffer Materials {Material m[]; };

// Get material by using PushConstant::materialId
// Material material = materials[pushConstant.materialId];


layout(binding = 2) uniform sampler2D[] textureSamplers;
// pack all textures together (diffuse, normal, height)
// Unpack by using Material::textureId to Offset
// vec3 diffuse = textureSamplers[material.diffuseMapId];
// vec3 normal = textureSamplers[material.normalMapId];
// vec3 height = textureSamplers[material.heightMapId];

layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    Material material = materials[pushConstant.materialId];
    // sampler2D diffuseMap = ;
    // outColor = vec4(1.0, 0.0, 0.0, 1.0);
    // outColor = vec4(0.0, fragTexCoord, 1.0);
    // outColor = vec4(normal, 1.0);
    outColor = vec4(texture(textureSamplers[material.diffuseMapId], fragTexCoord).rgb, 1.0);
    // outColor = vec4(texture(texSampler, fragTexCoord).rgb, 1.0);
}