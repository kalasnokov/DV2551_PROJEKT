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


}

terrainGenerator::terrainGenerator() {}


terrainGenerator::~terrainGenerator()
{
	compute.storageBuffer.destroy();
	compute.uniformBuffer.destroy();
	vkDestroyPipelineLayout(dataObjectptr->device, compute.pipelineLayout, nullptr);
	vkDestroyDescriptorSetLayout(dataObjectptr->device, compute.descriptorSetLayout, nullptr);
	vkDestroyPipeline(dataObjectptr->device, compute.pipeline, nullptr);
	vkDestroyFence(dataObjectptr->device, compute.fence, nullptr);
	vkDestroyCommandPool(dataObjectptr->device, compute.commandPool, nullptr);
}
