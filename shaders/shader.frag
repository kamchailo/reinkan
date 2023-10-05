#version 450

#extension GL_EXT_nonuniform_qualifier : enable
// #extension GL_EXT_buffer_reference2 : require

struct PushConstant
{
    mat4 modelMatrix;
	int objectId;
    int materialId;
    uint debugFlag;
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
    vec2 screenExtent;
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

struct ClusterGrid
{
    vec3	minPosition;
    uint    minPos_padding;
    vec3	maxPosition;
    uint    maxPos_padding;
};
layout(std140, binding = 4) readonly buffer ClusterGridBlock 
{
    ClusterGrid clusterGrids[];
};

struct ClusterPlane
{
    float	zNear;
    uint    zNear_padding;
    float	zFar;
    uint    zFar_padding;
};
layout(std140, binding = 5) readonly buffer ClusterPlaneBlock 
{
    ClusterPlane clusterPlanes[];
};

// LightIndex
layout(std140, binding = 6) readonly buffer LightIndexSSBO {
   uint lightIndexList[ ];
};

struct LightGrid
{
    uint offset;
    uint size;
};
layout(std140, binding = 7) readonly buffer LightGridSSBO {
   LightGrid lightGrids[ ];
};

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexTangent;
layout(location = 3) in vec3 vertexBitangent;
layout(location = 4) in vec3 viewDir;
layout(location = 5) in vec2 inFragTexCoord;

layout(location = 0) out vec4 outColor;

// include BRDF calculation
#include "brdf.glsl"
#include "parallax.glsl"

uint tileNumberX = 16;
uint tileNumberY = 9;
uint tileNumberZ = 32;

vec3 colorSample[8] = {vec3(1.0, 0.0, 0.0), 
                        vec3(1.0, 1.0, 0.0), 
                        vec3(0.0, 1.0, 0.0), 
                        vec3(0.0, 1.0, 1.0), 
                        vec3(0.0, 0.0, 1.0), 
                        vec3(1.0, 0.0, 1.0), 
                        vec3(0.0, 0.0, 0.0), 
                        vec3(1.0, 1.0, 1.0)}; 

float linearDepth(float depthSample, float zNear, float zFar)
{
    // float depthRange = 2.0 * depthSample - 1.0;
    // Near... Far... wherever you are...
    // float linear = 2.0 * zNear * zFar / (zFar + zNear - depthSample * (zFar - zNear));
    // after remove 2.0 * it fit the world depth
    float linear = zNear * zFar / (zFar + zNear - depthSample * (zFar - zNear));
    return linear;
}

void main() 
{
    Material material = materials[pushConstant.materialId];
    
    vec2 fragTexCoord = inFragTexCoord;

    if(material.diffuseMapId != -1)
    {
        vec4 diffuse = texture(textureSamplers[material.diffuseMapId], fragTexCoord);
        // overrride material
        material.diffuse = diffuse.rgb;
        if(diffuse.a == 0.0)
        {
            discard;
        }
    }

    vec3 N = normalize(vertexNormal);
    vec3 normalMap = texture(textureSamplers[material.normalMapId], fragTexCoord).rgb;
    if(material.normalMapId <= 200)
    {
        // N = normalize(normalMap * 2.0 - 1.0);
    }

    vec3 L = normalize(vec3(2.0, 5.0, 1.0) - worldPos);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 V = normalize(viewDir);
    
    vec3 ambient = vec3(0.1);
    vec3 diffuse = max(dot(N,L), 0.0) * lightColor;

    vec3 reflectDir = reflect(-L, N);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 7);
    vec3 specular = material.specular * spec * lightColor * 0.1; 
    
    vec3 phong = (ambient + diffuse + specular) * material.diffuse;

    outColor = vec4(phong, 1.0);
    // vec3 brdfColor = intensity * EvalBrdf(N, L, V, material);
    // outColor = vec4(brdfColor, 1.0);

    return;
}
