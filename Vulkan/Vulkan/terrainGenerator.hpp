#pragma once
#include <vulkan/vulkan.hpp>
#include "dataObjects.hpp"
#include "Computer.h"
#include <stdlib.h>
#include <time.h>
const int32_t chunk_size = 2048;
const size_t gpuMemoryRequired = ((chunk_size * chunk_size) * (sizeof(float)) * 9) / 1000000;
struct chunkArray
{
	float *buffer;
	glm::vec2 chunkID;
	const size_t mem = chunk_size * chunk_size * 9;
	chunkArray()
	{
		buffer = new float[mem];
	}
	~chunkArray()
	{
		delete buffer;
	}
};

class terrainGenerator
{
private:
	float seed;
	Computer *comp;

	Computer *meshComp;

	dataObjects *dataObjectptr;
	chunkArray chunkBuffer;

	int chunkSize;

	struct UBO { //Uniform Buffer Object
		float seed;
		uint32_t chunkSize;
		float idX;
		float idY;
	} ubo;

	struct VRO { //Vertex Return Object
		glm::vec3 vertex;
		float nan1; //Don't mind me
		glm::vec3 color;
		float nan2; //Don't mind me either, just fixing the alignment
	} vro;

public:

	void setUp(dataObjects *dataObjects, int chunkSize);
	std::vector<Vertex> generate(glm::vec2 chunkID);
	std::vector<uint32_t> generateIndices(int chunkSize);
	inline chunkArray *getChunkArray()
	{
		return &chunkBuffer;
	}
	terrainGenerator();
	~terrainGenerator();
};

