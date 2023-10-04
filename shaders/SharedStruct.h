#pragma once

#include <glm/glm.hpp>

namespace Reinkan
{
	constexpr uint32_t MAX_BONE_INFLUENCE{ 4 };

	constexpr uint32_t MAX_BONE{ 100 };

	struct Vertex
	{
		glm::vec3	position;
		glm::vec3	vertexNormal;
		glm::vec3	vertexTangent;
		glm::vec3	vertexBitangent;
		glm::vec2	texCoord;
		int32_t		boneIds[MAX_BONE_INFLUENCE]{ -1, -1, -1, -1 };
		float		boneWeights[MAX_BONE_INFLUENCE]{ -1.0, -1.0, -1.0, -1.0 };
	};

	struct UniformBufferObject 
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 viewInverse;
		alignas(16) glm::mat4 proj;
		alignas(8) glm::vec2 screenExtent;
	};

	struct AnimationUniformBufferObject
	{
		glm::mat4 boneMatrices[MAX_BONE];
	};

	struct PushConstantScanline
	{
		alignas(16) glm::mat4 modelMatrix;
		alignas(4) uint32_t objectId;
		alignas(4) uint32_t materialId;
		alignas(4) uint32_t debugFlag;
	};

	struct Material {
		alignas(16) glm::vec3 diffuse;
		alignas(16) glm::vec3 specular;
		alignas(4) uint32_t normalMapId;
		alignas(4) uint32_t heightMapId;
		alignas(4) float shininess;
		alignas(4) uint32_t diffuseMapId;
	};

	struct PushConstantDebug
	{
		alignas(16) glm::mat4 modelMatrix;
		alignas(16) glm::vec3 cameraPosition;
	};

	struct Particle
	{
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec4 color;
	};

	struct ComputeParticleUniformBufferObject
	{
		float deltaTime = 1.0f;
	};

	struct ComputeClusteredUniformBufferObject
	{
		alignas(16) glm::mat4 	view;
		alignas(16) glm::mat4 	proj;
		alignas(16) glm::mat4 	projInverse;
		alignas(8) glm::vec2 	screenDimensions;
		// alignas(4) uint32_t 	tileSizePx;
	};

	struct ClusterPlane
	{
		alignas(8) float	zNear;
		alignas(8) float	zFar;
	};

	struct ClusterGrid
	{
		alignas(16) glm::vec3	minPosition;
		alignas(16) glm::vec3	maxPosition;
	};

	struct LightObject
	{
		alignas(16) glm::vec3	position;
		alignas(16) glm::vec3	color;
		alignas(4) float 		intensity;
		alignas(4) float 		radius;
	};

	// LightGridIndex
	// uint	value;

	struct LightGrid
	{
		alignas(4) uint32_t offset;
		alignas(4) uint32_t size;
	};
}
