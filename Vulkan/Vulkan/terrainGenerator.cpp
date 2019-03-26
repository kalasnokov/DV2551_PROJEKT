#include "pch.h"
#include "terrainGenerator.hpp"
#include <limits>
#include <iostream>
#include <ctime>
#include <chrono>

void terrainGenerator::setUp(dataObjects * dataObjects, int chunkSize) {
	this->chunkSize = chunkSize;

	srand((unsigned)std::time(NULL));
	this->seed = ((float)rand()) / (float)RAND_MAX;
	dataObjectptr = dataObjects;

	int size = chunkSize * chunkSize * 9;
	std::vector<uint32_t> sizes;

	std::cout << "Seed: " << this->seed << "\n";
	std::cout << "Chunk size: " << chunkSize << "\n";
	ubo.seed = this->seed;
	ubo.chunkSize = (uint32_t)chunkSize;
	ubo.idX = 0;
	ubo.idY = 0;

	sizes.push_back(9 * chunkSize * chunkSize * sizeof(Vertex));
	sizes.push_back(sizeof(UBO));

	meshComp = new Computer(&dataObjectptr->device, &dataObjectptr->physicalDevice, &dataObjectptr->computeQueue, "../../Vulkan/Shaders/makeMesh.spv", sizes, glm::vec3(9, chunkSize, chunkSize));
	meshComp->populateBuffer(1, &ubo);
}

std::vector<Vertex> terrainGenerator::generate(glm::vec2 chunkID) {
	auto start = std::chrono::high_resolution_clock::now();
	ubo.idX = chunkID.x;
	ubo.idY = chunkID.y;

	meshComp->populateBuffer(1, &ubo);
	meshComp->run();

	VROResult = (Vertex*)meshComp->readBuffer(0);

	std::vector<Vertex> vertices(VROResult, VROResult + chunkSize * chunkSize * 9);

	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Generating world took: " << elapsed.count() << "s" << std::endl;

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
