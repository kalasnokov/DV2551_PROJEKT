#pragma once
#include <vulkan/vulkan.hpp>
#include "dataObjects.hpp"
#include "Computer.h"
const int32_t chunk_size = 2048;
const size_t gpuMemoryRequired = ((chunk_size * chunk_size) * (sizeof(float)) * 9) / 1000000;

class terrainGenerator
{
private:
	float seed;
	Computer *comp;
	dataObjects *dataObjectptr;
public:
	void setUp(dataObjects *dataObjects);
	void generate(int chunkID);
	terrainGenerator();
	~terrainGenerator();
};

