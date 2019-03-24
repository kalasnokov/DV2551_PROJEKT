#include "pch.h"
#include "terrainGenerator.hpp"
#include <limits>
#include <iostream>


void terrainGenerator::setUp(dataObjects * dataObjects)
{
	srand((unsigned)time_t(NULL));
	seed = std::numeric_limits<float>::min() + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (std::numeric_limits<float>::max() - std::numeric_limits<float>::min())));
	dataObjectptr = dataObjects;
	auto physicalDevice = dataObjects->physicalDevice;
	auto instance = dataObjects->instance;
	auto device = dataObjects->device;
	comp = new Computer(&device, &physicalDevice, &dataObjectptr->computeQueue, "../../Vulkan/Shaders/heightmap.spv", 1, (sizeof(float) * chunk_size * chunk_size) * 9);
	comp->run();
	float *result = new float[9 * chunk_size * chunk_size];
	result = (float*)comp->readOutBuffer();
	for (unsigned i = 0; i < 9; i++)
	{
		std::cout << result[i] << std::endl;
	}
}

void terrainGenerator::generate(int chunkID)
{
	comp->run();
}

terrainGenerator::terrainGenerator()
{

}


terrainGenerator::~terrainGenerator()
{
	delete comp;
}
