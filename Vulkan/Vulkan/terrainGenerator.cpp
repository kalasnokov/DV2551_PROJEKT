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
	float *input = new float[1];
	input[0] = 5;
	comp = new Computer(&device, &physicalDevice, &dataObjectptr->computeQueue, "../../Vulkan/Shaders/test.spv",sizeof(float), sizeof(float));
	comp->populateInBuffer(input);
	comp->run();
	float *result = new float[1];
	result = (float*)comp->readOutBuffer();
	std::cout << result[0] << std::endl;
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
