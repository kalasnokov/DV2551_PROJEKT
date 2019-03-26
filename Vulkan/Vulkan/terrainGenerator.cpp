#include "pch.h"
#include "terrainGenerator.hpp"
#include <limits>
#include <iostream>
#include <ctime>
#include <chrono>

void terrainGenerator::setUp(dataObjects * dataObjects, int chunkSize) {
	this->chunkSize = chunkSize;

	srand((unsigned)std::time(NULL));
	this->seed = rand() % 100000;
	dataObjectptr = dataObjects;

	int size = chunkSize * chunkSize * 9;
	std::vector<uint32_t> sizes;
	sizes.push_back(size * sizeof(float));

	std::cout << "Seed: " << this->seed << "\n";
	std::cout << "Chunk size: " << chunkSize << "\n";
	ubo.seed = this->seed;
	ubo.chunkSize = (uint32_t)chunkSize;
	ubo.idX = 0;
	ubo.idY = 0;

	sizes.push_back(sizeof(ubo));

	comp = new Computer(&dataObjectptr->device, &dataObjectptr->physicalDevice, &dataObjectptr->computeQueue, "../../Vulkan/Shaders/test.spv", sizes, glm::vec3(9, 1, 1));
	comp->populateBuffer(1, &ubo);

	sizes.clear();
	sizes.push_back(size * sizeof(float));
	sizes.push_back(9 * chunkSize * chunkSize * sizeof(VRO));
	sizes.push_back(sizeof(CMUBO));

	cmubo.chunkSize = (uint32_t)chunkSize;
	cmubo.realWorldDistance = (uint32_t)10;

	meshComp = new Computer(&dataObjectptr->device, &dataObjectptr->physicalDevice, &dataObjectptr->computeQueue, "../../Vulkan/Shaders/makeMesh.spv", sizes, glm::vec3(9, chunkSize, chunkSize));
	meshComp->populateBuffer(2, &cmubo);
}

std::vector<Vertex> terrainGenerator::generate(glm::vec2 chunkID) {
	//Set chunkID as parameter allong with seed
	//Chunk ID is where in the grid the player is
	auto start = std::chrono::high_resolution_clock::now();
	//comp->run();
	float *result = new float[chunkSize * chunkSize * 9];
	result = (float*)comp->readBuffer(0);
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nCompute shader result:\n";
	/*
	for (int i = 0; i < 9; i++) {
		std::cout << result[i] << " ";
	}
	std::cout << "\n\n";
	for (int i = 0; i < 9; i++) {
		for (int x = 0; x < chunkSize; x++) {
			for (int y = 0; y < chunkSize; y++) {
				std::cout << result[i * chunkSize * chunkSize + x * chunkSize + y] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	*/
	meshComp->populateBuffer(0, result);
	meshComp->populateBuffer(2, &ubo);
	meshComp->run();

	VRO* VROResult = new VRO[chunkSize * chunkSize];
	VROResult = (VRO*)meshComp->readBuffer(1);
	
	for (int i = 0; i < 10; i++) {
		//std::cout << "(" << VROResult[i].vertex.x << ", " << VROResult[i].vertex.y << ", " << VROResult[i].vertex.z << "), (" << VROResult[i].color.x << ", " << VROResult[i].color.y << ", " << VROResult[i].color.z << ")\n";
	}
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Generating world took: " << elapsed.count() << "s" << std::endl;
	/*
	std::cout << "\n";
	for (int i = 0; i < 9 * chunkSize * chunkSize; i++) {
		if (VROResult[i].vertex.x != 0 || VROResult[i].vertex.y != 0 || VROResult[i].vertex.z != 0) {
			std::cout << "(" << VROResult[i].vertex.x << ", " << VROResult[i].vertex.y << ", " << VROResult[i].vertex.z << "), (" << VROResult[i].color.x << ", " << VROResult[i].color.y << ", " << VROResult[i].color.z << ")\n";
		}
	}
	*/
	std::vector<Vertex> vertices;
	for (int i = 0; i < 9 * chunkSize * chunkSize; i++) {
		vertices.push_back({ {VROResult[i].vertex.x, VROResult[i].vertex.y, VROResult[i].vertex.z}, {VROResult[i].color.x, VROResult[i].color.y, VROResult[i].color.z} });
	}
	
	return vertices;
}

std::vector<uint32_t> terrainGenerator::generateIndices(int chunkSize) {
	std::vector<uint32_t> indices;
	int quads = chunkSize - 1;

	std::cout << "CS: " << chunkSize << "\n";
	std::cout << "Q: " << quads << "\n";

	for (int ty = 0; ty < 3; ty++) {
		int tyPos = ty * chunkSize * chunkSize * 3;
		for (int tx = 0; tx < 3; tx++) {
			int txPos = tx * chunkSize * chunkSize;
			for (int vy = 0; vy < quads; vy++) {
				int vyPos = vy * chunkSize;
				for (int vx = 0; vx < quads; vx++) {
					indices.push_back(tyPos + txPos + vyPos + chunkSize + vx);
					indices.push_back(tyPos + txPos + vyPos + vx);
					indices.push_back(tyPos + txPos + vyPos + chunkSize + vx + 1);
					indices.push_back(tyPos + txPos + vyPos + chunkSize + vx + 1);
					indices.push_back(tyPos + txPos + vyPos + vx);
					indices.push_back(tyPos + txPos + vyPos + vx + 1);
				}
			}
		}
	}
	return indices;
}

terrainGenerator::terrainGenerator()
{

}


terrainGenerator::~terrainGenerator()
{
	delete comp;
}
