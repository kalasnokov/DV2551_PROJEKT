#pragma once
#include <vulkan/vulkan.hpp>
#include "dataObjects.hpp"
#include "Computer.h"
const int32_t chunk_size = 2048;
const size_t gpuMemoryRequired = ((chunk_size * chunk_size) * (sizeof(float)) * 9) / 1000000;
struct chunkArray
{
	float *buffer;
	glm::vec2 chunkID;
	chunkArray()
	{
		buffer = new float[9 * chunk_size * chunk_size];
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
	dataObjects *dataObjectptr;
	chunkArray chunkBuffer;
public:
	void setUp(dataObjects *dataObjects);
	void generate(glm::vec2 chunkID);
	inline chunkArray *getChunkArray()
	{
		return &chunkBuffer;
	}
	terrainGenerator();
	~terrainGenerator();
};

