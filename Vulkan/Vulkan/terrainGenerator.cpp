#include "pch.h"
#include "terrainGenerator.hpp"



void terrainGenerator::setUp(dataObjects * dataObjects)
{
	dataObjectptr = dataObjects;
	auto physicalDevice = dataObjects->physicalDevice;
	auto instance = dataObjects->instance;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
}

terrainGenerator::terrainGenerator() {}


terrainGenerator::~terrainGenerator()
{
	//Unmap memory
}
