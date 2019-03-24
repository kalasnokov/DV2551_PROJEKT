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
	std::cout << "\nCompute shader result: " << result[0] <<"\n\n";
}

void terrainGenerator::generate(glm::vec2 chunkID)
{
	//Set chunkID as parameter allong with seed
	//Chunk ID is where in the grid the player is
	comp->run();
	chunkBuffer.buffer = (float*)comp->readOutBuffer();
}

terrainGenerator::terrainGenerator()
{

}


terrainGenerator::~terrainGenerator()
{
	delete comp;
}
