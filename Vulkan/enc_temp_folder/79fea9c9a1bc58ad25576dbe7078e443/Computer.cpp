#include "pch.h"
#include "Computer.h"


Computer::Computer(VkDevice* device, VkPhysicalDevice* physicalDevice, std::string shaderLoc, uint32_t inBufSize, uint32_t outBufSize, uint32_t height, uint32_t width, uint32_t workForceGroupSize){
	this->device = device;
	this->physicalDevice = physicalDevice;

	this->height = height;
	this->width = width;
	this->workForceGroupSize = workForceGroupSize;

	inBufferSize = inBufSize;
	outBufferSize = outBufSize;

	queueFamilyIndex = VHF::findQueueFamily(*physicalDevice, VK_QUEUE_COMPUTE_BIT);
	std::cout << "COMPUTER: QUEUEFAMILYINDEX SET!\n";
	setupBuffers();
	std::cout << "COMPUTER: BUFFERS SET!\n";
	loadShader(shaderLoc);
	std::cout << "COMPUTER: SHADER LOADED!\n";
	descriptorSetup();
	std::cout << "COMPUTER: DESCRIPTORS SET!\n";
	pipelineSetup();
	std::cout << "COMPUTER: PIPELINE SET!\n";
	commandBufferSetup();
	std::cout << "COMPUTER: COMMANDBUFFER SET!\n";
}

void Computer::setupBuffers() {
	VkBufferCreateInfo bufferCreateInfo[2];
	bufferCreateInfo[0].sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo[0].size = inBufferSize;
	bufferCreateInfo[0].usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	bufferCreateInfo[0].sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	bufferCreateInfo[1].sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo[1].size = outBufferSize;
	bufferCreateInfo[1].usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	bufferCreateInfo[1].sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VHF::createBuffer(*device, *physicalDevice, inBufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, inBuffer, inBufferMem);
	VHF::createBuffer(*device, *physicalDevice, outBufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, outBuffer, outBufferMem);
}

void Computer::commandBufferSetup() {
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = 0;
	commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

	if (vkCreateCommandPool(*device, &commandPoolCreateInfo, NULL, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO CREATE COMMANDPOOL!");
	}

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(*device, &commandBufferAllocateInfo, &commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO ALLOCATE COMMAND BUFFERS!");
	}

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSet, 0, NULL);

		vkCmdDispatch(commandBuffer, (uint32_t)ceil(width / float(workForceGroupSize)), (uint32_t)ceil(height / float(workForceGroupSize)), 1);

	vkEndCommandBuffer(commandBuffer);
}

void Computer::descriptorSetup() {
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding[2];
	descriptorSetLayoutBinding[0].binding = 0;
	descriptorSetLayoutBinding[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptorSetLayoutBinding[0].descriptorCount = 1;
	descriptorSetLayoutBinding[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

	descriptorSetLayoutBinding[1].binding = 1;
	descriptorSetLayoutBinding[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptorSetLayoutBinding[1].descriptorCount = 1;
	descriptorSetLayoutBinding[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.bindingCount = 2;
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBinding;

	// Create the descriptor set layout. 
	if (vkCreateDescriptorSetLayout(*device, &descriptorSetLayoutCreateInfo, NULL, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO SETUP DESCRIPTOR!");
	}
}

void Computer::pipelineSetup() {
	VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {};
	shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	shaderStageCreateInfo.module = shader;
	shaderStageCreateInfo.pName = "main";

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;

	if (vkCreatePipelineLayout(*device, &pipelineLayoutCreateInfo, NULL, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO SETUP PIPELINE LAYOUT!");
	}

	VkComputePipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stage = shaderStageCreateInfo;
	pipelineCreateInfo.layout = pipelineLayout;

	if (vkCreateComputePipelines(*device, VK_NULL_HANDLE, 1, &pipelineCreateInfo,NULL, &pipeline) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO CREATE PIPELINE!");
	}
}

void Computer::loadShader(std::string shaderLoc) {
	auto code = VHF::readFile(shaderLoc);
	shader = VHF::createShaderModule(*device, code);
}

Computer::~Computer() {

}