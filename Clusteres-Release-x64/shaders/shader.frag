#version 450

#extension GL_EXT_nonuniform_qualifier : enable
// #extension GL_EXT_buffer_reference2 : require

struct PushConstant
{
    mat4 modelMatrix;
	int objectId;
    int materialId;
    uint debugFlag;
    float debugFloat;
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

float LinearDepth(float depthSample, float zNear, float zFar)
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

    ////////////////////////////////////////
    //          Parallax Occlusion
    ////////////////////////////////////////
    // if(material.heightMapId != -1)
    if(false)
    {
        mat3 TBN = transpose(mat3(vertexTangent, 
                                 vertexBitangent,
                                 vertexNormal));

        vec3 viewPos = vec3(ubo.viewInverse * vec4(0, 0, 0, 1));

        vec3 TangentViewPos  = TBN * viewPos;
        vec3 TangentFragPos  = TBN * worldPos;

        vec3 viewD = normalize(TangentViewPos - TangentFragPos);

        float height =  1.0 - texture(textureSamplers[material.heightMapId], fragTexCoord).r;
    
        // fragTexCoord = ParallaxMapping(fragTexCoord, viewD, height);
        fragTexCoord = HighParallaxMapping(fragTexCoord, viewD, textureSamplers[material.heightMapId]);
        
        if(fragTexCoord.x > 1.0 || fragTexCoord.y > 1.0 || fragTexCoord.x < 0.0 || fragTexCoord.y < 0.0)
        {
            discard;
        }
    }

    if(material.diffuseMapId != -1)
    {
        vec3 diffuse = texture(textureSamplers[material.diffuseMapId], fragTexCoord).rgb;
        // overrride material
        material.diffuse = diffuse;
    }

    vec3 N = normalize(vertexNormal);
    vec3 normalMap = texture(textureSamplers[material.normalMapId], fragTexCoord).rgb;
    if(material.normalMapId <= 200)
    {
        // N = normalize(normalMap * 2.0 - 1.0);
    }

    // Main Directional Light
    vec3 L = normalize(vec3(1.0, 3.0, 1.0) - worldPos);
    float ambientLight = 0.6;
    float intensity = 0.7;
    vec3 V = normalize(viewDir);
    vec3 brdfColor = intensity * EvalBrdf(N, L, V, material);
    
    ////////////////////////////////////////
    //          Grid Calculation
    ////////////////////////////////////////
    // Determine which Grid for this fragment
    float z = length(viewDir);
    float zNear = clusterPlanes[0].zNear;
    float zFar = clusterPlanes[tileNumberZ - 1].zFar;

    float linear = LinearDepth(gl_FragCoord.z, zNear, zFar);
    
    float aTerm = float(tileNumberZ) / log(zFar/ zNear);

    // linear = pow(linear,  pushConstant.debugFloat);

    // float diff = z - gl_FragCoord.z;

    // outColor = vec4(vec3(diff), 1.0);

    // return;

    // Final Z Plane
    // uint slice = uint(log(z) * (aTerm) - aTerm * log(zNear));
    uint sliceFlat = uint(log(linear) * (aTerm) - aTerm * log(zNear));

    uint tileSizeX = uint(ubo.screenExtent.x / tileNumberX);
    uint tileSizeY = uint(ubo.screenExtent.y / tileNumberY);
    
    uvec3 tiles = uvec3( gl_FragCoord.x / tileSizeX, 
                         gl_FragCoord.y /tileSizeY, 
                         sliceFlat );

    uint tileIndex = tiles.x +
                     tileNumberX * tiles.y +
                     (tileNumberX * tileNumberY) * tiles.z;

    LightGrid lightGrid = lightGrids[tileIndex];
    uint offset = lightGrid.offset;
    // Loop through all light assigned in the grid
    for(int i = 0; i < lightGrid.size; ++i)
    {
        uint lightIndex = lightIndexList[offset + i];
        LightObject light = globalLights[lightIndex];

        float lightDistance = distance(light.position, worldPos);
        if(lightDistance >= light.radius)
        {
            continue;
        }
        L = normalize(light.position - worldPos);

        if((pushConstant.debugFlag & 0x4) > 1)
        {
            float intensity = light.intensity;
            brdfColor += intensity * 0.2 * light.color;
        }
        else
        {
            float intensity = light.intensity * (1 - lightDistance / light.radius);
            brdfColor += intensity * light.color * EvalBrdf(N, L, V, material);
        }
    }


    ////////////////////////////////////////
    //          Debug Flag
    ////////////////////////////////////////

    if(lightGrid.size > 0 && (pushConstant.debugFlag & 0x1) > 0)
    {
        brdfColor += vec3(float(lightGrid.size)/ 50);
    }

    outColor = vec4(brdfColor, 1.0);
    if((pushConstant.debugFlag & 0x2) > 0)
    {
        uint colorIndex = sliceFlat % 8;
        // outColor += vec4(vec3(colorSample[colorIndex]), 0.3);
        outColor += vec4(colorSample[colorIndex] * 0.1, 1.0);
        // outColor += vec4(tiles.x / 16.0, tiles.y / 9.0, 0.0, 0.1);
    }
}
