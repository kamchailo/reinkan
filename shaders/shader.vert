#version 450

#extension GL_EXT_nonuniform_qualifier : enable

struct PushConstant
{
    mat4 modelMatrix;
    mat4 normalMatrix;
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

layout(binding = 8) uniform AnimationMatrices 
{
   mat4 matrices[100];
} animationMatrices;

layout(location = 0) in vec3    inPosition;
layout(location = 1) in vec3    inVertexNormal;
layout(location = 2) in vec3    inVertexTangent;
layout(location = 3) in vec3    inVertexBitangent;
layout(location = 4) in vec2    inTexCoord;
layout(location = 5) in int     inBoneId[4];
layout(location = 9) in float   inBoneWeight[4];

layout(location = 0) out vec3 worldPos;
layout(location = 1) out vec3 vertexNormal;
layout(location = 2) out vec3 vertexTangent;
layout(location = 3) out vec3 vertexBitangent;
layout(location = 4) out vec3 viewDir;
layout(location = 5) out vec2 fragTexCoord;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

void main() 
{
    //////////////////////////////
    //      Bone Transformation
    //////////////////////////////
    vec4 totalPosition = vec4(0.0f);
    
    vec3 localNormal;
    
    int boneInfluence = 0;
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(inBoneId[i] < 0) 
            continue;
        if(inBoneId[i] >= MAX_BONES) 
        {
            totalPosition = vec4(inPosition,1.0f);
            break;
        }
        ++boneInfluence;
        vec4 localPosition = animationMatrices.matrices[inBoneId[i]] * vec4(inPosition,1.0f);
        totalPosition += localPosition * inBoneWeight[i];
        localNormal = mat3(animationMatrices.matrices[inBoneId[i]]) * inVertexNormal;
    }
    
    if(boneInfluence == 0)
    {
        totalPosition = vec4(inPosition,1.0f);
        localNormal = inVertexNormal;
    }

    mat4 modelTransform = ubo.proj * ubo.view * pushConstant.modelMatrix;
    
    mat4 normalTransform = pushConstant.normalMatrix;
    

    gl_Position =  modelTransform * totalPosition;
    
    vec3 eye = ubo.viewInverse[3].xyz;

    // out
    worldPos = vec3(pushConstant.modelMatrix * totalPosition);
    vertexNormal = normalize((normalTransform * vec4(localNormal, 1.0))).rgb;
    vertexTangent = normalize((normalTransform * vec4(inVertexTangent, 1.0))).rgb;
    vertexBitangent = normalize((normalTransform * vec4(inVertexBitangent, 1.0))).rgb;
    viewDir = vec3(eye - worldPos);
    fragTexCoord = inTexCoord;
}