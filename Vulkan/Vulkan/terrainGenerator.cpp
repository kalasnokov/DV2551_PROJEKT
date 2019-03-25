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

	std::vector<uint32_t> sizes;
	int size = 5;
	float *input = new float[size];
	srand(time(NULL));

	for (int i = 0; i < size; i++) {
		input[i] = rand() % 100;
	}

	std::vector<uint32_t> sizes;
	for (int i = 0; i < 2; i++) {
		sizes.push_back(sizeof(float) * size);
	}

	struct UBO {
		float value = 3.f;
		float value2 = 2.f;
	};

	UBO ubo;
	sizes.push_back(sizeof(UBO));

	comp = new Computer(&device, &physicalDevice, &dataObjectptr->computeQueue, "../../Vulkan/Shaders/test.spv", sizes);
	comp->populateBuffer(0, input);
	comp->populateBuffer(2, &ubo);
	comp->run();

	float *result = new float[size];
	result = (float*)comp->readBuffer(1);

	std::cout << "\nCompute expected result:\n";
	for (int i = 0; i < size; i++) {
		std::cout << (input[i] + ubo.value) * ubo.value2 << " ";
	}

	std::cout << "\nCompute shader result:\n";
	for (int i = 0; i < size; i++) {
		std::cout << result[i] << " ";
	}
	std::cout << "\n";
	/*struct UBO
	{
		float seed;
		glm::vec2 chunkID;
	}ubo;
	ubo.seed = this->seed;
	ubo.chunkID = this->chunkBuffer.chunkID;
	sizes.push_back(sizeof(UBO));
	comp = new Computer(&device, &physicalDevice, &dataObjectptr->computeQueue, "../../Vulkan/Shaders/heightmap.spv", sizes);
	comp->populateBuffer(2, &ubo);
	comp->run();
	chunkBuffer.buffer = (float*)comp->readBuffer(1);*/
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
