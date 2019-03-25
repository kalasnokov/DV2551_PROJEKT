#pragma once
#include <vulkan/vulkan.hpp>
#include "VHF.h"

class Computer
{
public:
	Computer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkQueue* queue, std::string shaderLoc, std::vector<uint32_t> bufferSizes, glm::vec3 workForce);

	void * readBuffer(int loc);

	void populateBuffer(int loc, const void* src);

	void run();

	void setupBuffers();

	void commandBufferSetup();

	void descriptorLayoutSetup();

	void descriptorSetup();

	void pipelineSetup();

	void loadShader(std::string shaderLoc);

	struct bufferInfo {
	public:
		VkBuffer buffer;
		VkDeviceMemory bufferMem;
		uint32_t bufferSize;
	};

	~Computer();

private:
	VkDevice* device;
	VkPhysicalDevice* physicalDevice;

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

	std::vector<bufferInfo> bufferStructs;

	glm::vec3 workForce;
};

