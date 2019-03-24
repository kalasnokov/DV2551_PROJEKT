#pragma once
#include <vulkan/vulkan.hpp>
#include "VHF.h"

class Computer
{
public:
	Computer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkQueue* queue, std::string shaderLoc, uint32_t inBufSize, uint32_t outBufSize);

	void populateInBuffer(const void* src);

	void* readOutBuffer();

	void run();

	void setupBuffers();

	void commandBufferSetup();

	void descriptorLayoutSetup();

	void descriptorSetup();

	void pipelineSetup();

	void loadShader(std::string shaderLoc);

	~Computer();

private:
	VkDevice* device;
	VkPhysicalDevice* physicalDevice;

	VkBuffer inBuffer;
	VkBuffer outBuffer;

	uint32_t inBufferSize;
	uint32_t outBufferSize;

	VkDeviceMemory inBufferMem;
	VkDeviceMemory outBufferMem;

	VkBuffer uniforms;

	VkQueue* queue;
	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;
	VkFence fence;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSet descriptorSet;
	VkDescriptorPool descriptorPool;

	VkPipelineLayout pipelineLayout;
	VkPipeline pipeline;
	int32_t pipelineIndex = 0;

	uint32_t queueFamilyIndex;

	VkShaderModule shader;

	uint32_t height;
	uint32_t width;
	uint32_t workForceGroupSize;
};
