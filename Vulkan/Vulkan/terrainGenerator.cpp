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
	srand(time(NULL));
	input[0] = rand() % 100;

	std::vector<uint32_t> sizes;
	for (int i = 0; i < 2; i++) {
		sizes.push_back(sizeof(float));
	}

	struct UBO {
		float value = 42.f;
		float value2 = 33.f;
	};

	UBO ubo;
	sizes.push_back(sizeof(UBO));

	comp = new Computer(&device, &physicalDevice, &dataObjectptr->computeQueue, "../../Vulkan/Shaders/test.spv", sizes);
	comp->populateBuffer(0, input);
	comp->populateBuffer(2, &ubo);
	comp->run();
	float *result = new float[1];
	result = (float*)comp->readBuffer(1);
	std::cout << "\nCompute expected result: " << input[0] + 1 <<"\n\n";
	std::cout << "\nCompute shader result: " << result[0] << "\n\n";
}

void terrainGenerator::generate(glm::vec2 chunkID)
{
	//Set chunkID as parameter allong with seed
	//Chunk ID is where in the grid the player is
	comp->run();
	//chunkBuffer.buffer = (float*)comp->readOutBuffer();
}

terrainGenerator::terrainGenerator()
{

}


terrainGenerator::~terrainGenerator()
{
	delete comp;
}
