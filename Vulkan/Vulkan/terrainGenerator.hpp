#pragma once
#include <vulkan/vulkan.hpp>

const size_t chunk_size = 64;
const size_t max_chunks = 100;

class terrainGenerator
{
private:
	struct memory
	{

	};
public:
	void setDevice(VkDevice &dev);
	void setUp();
	terrainGenerator();
	~terrainGenerator();
};

