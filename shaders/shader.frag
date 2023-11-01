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
    float debugFloat2;
    int debugInt;
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
    uint pyramidalHeightMapId;
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

layout(binding = 8) uniform sampler2D[] pyramidalSamplers;

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexTangent;
layout(location = 3) in vec3 vertexBitangent;
layout(location = 4) in vec3 viewDir;
layout(location = 5) in vec2 inFragTexCoord;
// Parallax
layout(location = 6) in vec3 TBNViewPos;
layout(location = 7) in vec3 TBNWorldPos;
layout(location = 8) in mat3 TBNMatrix;

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
    if(pushConstant.materialId == 1)
    {
        int MAX_LEVEL = 11;
        vec3 E = normalize(TBNMatrix * viewDir);
        E.x = -E.x;

        // Make E.z = 1 for easiser calculation
        E = E / E.z;

        vec3 pOrigin = vec3(fragTexCoord, 0.0);
        
        // Cast forward once to first level
        float depth = texture(pyramidalSamplers[MAX_LEVEL], fragTexCoord).r * pushConstant.debugFloat ;
        vec3 pPrime = pOrigin + E * depth;

        int MAX_ITERATION = 30;
        int minLevel = clamp(pushConstant.debugInt, 0, MAX_LEVEL);
        
        for (int level = MAX_LEVEL - 1; level >= minLevel; )
        {
            if(--MAX_ITERATION <= 0)
            {
                break;
                // outColor = vec4(0.7, 0.3, 1.0, 1.0);
                // return;
            }

            // Refinement
            /*            
            if(level <= 0 )
            {
                vec2 ray2D = pPrime.xy - pOrigin.xy;
                float rayLength = length(ray2D);
                float texelSpanHalf = 0.5 / pow(2.0, MAX_LEVEL - level);
                float depthB = pPrime.z * (rayLength + texelSpanHalf) / rayLength;
                
                vec3 pPrimeB = pOrigin + E * depthB;
                float depthPrimeB = 1.0 - (texture(textureSamplers[material.heightMapId], pPrimeB.xy).r * pushConstant.debugFloat);
                
                if(depthPrimeB > pPrimeB.z)
                {
                    pPrime = pPrimeB;
                    continue;
                }
            }
            */

            float depth = texture(pyramidalSamplers[level], pPrime.xy).r * pushConstant.debugFloat;
            
            if(depth > pPrime.z)
            {
                vec3 pTemp = pOrigin + E * depth;
                
                // Check for node crossing
                float nodeCount = pow(2.0, MAX_LEVEL - level);
                
                vec2 nodePPrime = floor(pPrime.xy * nodeCount);
                vec2 nodePTemp = floor(pTemp.xy * nodeCount);
                
                vec2 test = abs(nodePPrime - nodePTemp);

                // Cross the pixel
                if(test.x + test.y > 0.001)
                {
                    float texelSpan = 1.0 / nodeCount;
                    
                    vec2 dirSign = (sign(E.xy) + 1.0) * 0.5; // {0, 1}
                    
                    // distance to the next node's boundary
                    vec2 pBoundary = (nodePPrime + dirSign) * texelSpan;
                    vec2 a = pPrime.xy    - pOrigin.xy;
                    vec2 b = pBoundary.xy - pOrigin.xy;
                    
                    // Fix bug when depth start at 0
                    if( a == vec2(0))  a = vec2(0.001);
                    
                    // node crossing
                    vec2 depthAtBoundary = (pPrime.z * b.xy) / a.xy;
                    
                    float offset = texelSpan * 0.001;
                    
                    depth = min(depthAtBoundary.x, depthAtBoundary.y) + offset;
                    
                    // Move pPrime to boundary
                    pPrime = pOrigin + E * depth;
                }
                else
                {
                    pPrime = pTemp;
                    --level;
                }
            }
            else
            {
                --level;
            }
        }

        vec2 parallaxUV = (pPrime).xy;
        if(parallaxUV.x < 0.0 || parallaxUV.y < 0.0 || parallaxUV.x > 1.0 || parallaxUV.y > 1.0)
        {
            discard;
        }

        fragTexCoord = parallaxUV;

        if((pushConstant.debugFlag & 0x2) > 0)
        {
            float depthColor = texture(pyramidalSamplers[minLevel], fragTexCoord).r;
            outColor = vec4(vec3(depthColor), 1.0);
            return;
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
        normalMap = normalMap * 2.0 - 1.0;
        N = abs(normalize(TBNMatrix * normalMap));
        
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
    // Define necessary variables
    float zNear = clusterPlanes[0].zNear;
    float zFar = clusterPlanes[tileNumberZ - 1].zFar;
    float linear = LinearDepth(gl_FragCoord.z, zNear, zFar);
    float aTerm = float(tileNumberZ) / log(zFar/ zNear);
    // Z Plane of current Grid
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

    // Final Color Result
    outColor = vec4(brdfColor, 1.0);


    ////////////////////////////////////////
    //          Debug Flag
    ////////////////////////////////////////
    if((pushConstant.debugFlag & 0x1) > 0)
    {

    }
}
