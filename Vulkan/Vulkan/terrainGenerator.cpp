#include "pch.h"
#include "terrainGenerator.hpp"
#include <limits>
#include <iostream>
#include <ctime>

void terrainGenerator::setUp(dataObjects * dataObjects)
{
	srand((unsigned)std::time(NULL));
	this->seed = rand() % 100000;
	dataObjectptr = dataObjects;
	auto physicalDevice = dataObjects->physicalDevice;
	auto instance = dataObjects->instance;
	auto device = dataObjects->device;

	int size = 8 * 8 * 9;
	std::vector<uint32_t> sizes;
	sizes.push_back(size * sizeof(float));

	struct UBO {
		float seed;
		uint32_t idX;
		uint32_t idY;
	} ubo;
	std::cout << "Seed: " << this->seed << "\n";
	ubo.seed = this->seed;
	ubo.idX = 2;
	ubo.idY = 3;

	sizes.push_back(sizeof(UBO));

	comp = new Computer(&device, &physicalDevice, &dataObjectptr->computeQueue, "../../Vulkan/Shaders/test.spv", sizes);
	comp->populateBuffer(1, &ubo);
	comp->run();

	float *result = new float[size];
	result = (float*)comp->readBuffer(0);

	std::cout << "\nCompute shader result:\n";
	for (int i = 0; i < 9; i++) {
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				std::cout << result[i + x * 8 + y] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
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
