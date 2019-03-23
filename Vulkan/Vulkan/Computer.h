#pragma once
#include <vulkan/vulkan.hpp>
#include "VHF.h"

class Computer
{
public:
	Computer(VkDevice* device, VkPhysicalDevice* physicalDevice, std::string shaderLoc);

	void commandBufferSetup();

	void descriptorSetup();

	void pipelineSetup();

	void loadShader(std::string shaderLoc);

	~Computer();

private:
	VkDevice* device;
	VkPhysicalDevice* physicalDevice;

	VkBuffer inData;
	VkBuffer uniforms;

	VkQueue queue;
	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;
	VkFence fence;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSet descriptorSet;

	VkPipelineLayout pipelineLayout;
	std::vector<VkPipeline> pipelines;
	int32_t pipelineIndex = 0;

	uint32_t queueFamilyIndex;

	VkShaderModule shader;
};

