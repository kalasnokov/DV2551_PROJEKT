#include "pch.h"
#include "Computer.h"


Computer::Computer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkQueue* queue, std::string shaderLoc, uint32_t inBufSize, uint32_t outBufSize){
	this->device = device;
	this->physicalDevice = physicalDevice;
	this->queue = queue;

	this->height = height;
	this->width = width;
	this->workForceGroupSize = workForceGroupSize;

	this->inBufferSize = inBufSize;
	this->outBufferSize = outBufSize;

	queueFamilyIndex = VHF::findQueueFamily(*physicalDevice, VK_QUEUE_COMPUTE_BIT);
	std::cout << "COMPUTER: QUEUEFAMILYINDEX SET!\n";
	setupBuffers();
	std::cout << "COMPUTER: BUFFERS SET!\n";
	loadShader(shaderLoc);
	std::cout << "COMPUTER: SHADER LOADED!\n";
	descriptorLayoutSetup();
	std::cout << "COMPUTER: DESCRIPTOR LAYOUTS SET!\n";
	pipelineSetup();
	std::cout << "COMPUTER: PIPELINE SET!\n";
	descriptorSetup();
	std::cout << "COMPUTER: DESCRIPTORS SET!\n";
	commandBufferSetup();
	std::cout << "COMPUTER: COMMANDBUFFER SET!\n";
}

void Computer::run() {
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VkFence fence;
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = 0;

	if (vkCreateFence(*device, &fenceCreateInfo, NULL, &fence) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO CREATE FENCE!");
	}

	if (vkQueueSubmit(*queue, 1, &submitInfo, fence) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO SUBMIT QUEUE!");
	}

	if (vkWaitForFences(*device, 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max()) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO WAIT FOR FENCE! (HOW???)");
	}

	vkDestroyFence(*device, fence, NULL);
}

void Computer::setupBuffers() {
	VkBufferCreateInfo inBufferCreateInfo = {};
	inBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	inBufferCreateInfo.size = inBufferSize;
	inBufferCreateInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	inBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBufferCreateInfo outBufferCreateInfo = {};
	outBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	outBufferCreateInfo.size = inBufferSize;
	outBufferCreateInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	outBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VHF::createBuffer(*device, *physicalDevice, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, inBuffer, inBufferMem, inBufferCreateInfo);
	VHF::createBuffer(*device, *physicalDevice, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, outBuffer, outBufferMem, outBufferCreateInfo);
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

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO BEGIN COMMAND BUFFER!");
	}

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSet, 0, NULL);

		vkCmdDispatch(commandBuffer, 1, 1, 1);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO END COMMAND BUFFER!");
	}
}

void Computer::descriptorLayoutSetup() {
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
	if (vkCreateDescriptorSetLayout(*device, &descriptorSetLayoutCreateInfo, 0, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO SETUP DESCRIPTORLAYOUT!");
	}
}

void Computer::descriptorSetup() {
	VkDescriptorPoolSize descriptorPoolSize = {};
	descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptorPoolSize.descriptorCount = 2;

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.maxSets = 1;
	descriptorPoolCreateInfo.poolSizeCount = 1;
	descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;

	if (vkCreateDescriptorPool(*device, &descriptorPoolCreateInfo, NULL, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO SETUP DESCRIPTOR POOL!");
	}

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.descriptorPool = descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = 1;
	descriptorSetAllocateInfo.pSetLayouts = &descriptorSetLayout;

	if (vkAllocateDescriptorSets(*device, &descriptorSetAllocateInfo, &descriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO SETUP DESCRIPTOR SETS!");
	}

	VkDescriptorBufferInfo inInfo;
	inInfo.buffer = inBuffer;
	inInfo.offset = 0;
	inInfo.range = VK_WHOLE_SIZE;

	VkDescriptorBufferInfo outInfo;
	outInfo.buffer = outBuffer;
	outInfo.offset = 0;
	outInfo.range = VK_WHOLE_SIZE;

	
	VkWriteDescriptorSet writeDescriptorSet[2];
	writeDescriptorSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet[0].pNext = 0;
	writeDescriptorSet[0].dstSet = descriptorSet;
	writeDescriptorSet[0].dstBinding = 0;
	writeDescriptorSet[0].dstArrayElement = 0;
	writeDescriptorSet[0].descriptorCount = 1;
	writeDescriptorSet[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	writeDescriptorSet[0].pImageInfo = 0;
	writeDescriptorSet[0].pBufferInfo = &inInfo;
	writeDescriptorSet[0].pTexelBufferView = 0;

	writeDescriptorSet[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet[1].pNext = 0;
	writeDescriptorSet[1].dstSet = descriptorSet;
	writeDescriptorSet[1].dstBinding = 1;
	writeDescriptorSet[1].dstArrayElement = 0;
	writeDescriptorSet[1].descriptorCount = 1;
	writeDescriptorSet[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	writeDescriptorSet[1].pImageInfo = 0;
	writeDescriptorSet[1].pBufferInfo = &inInfo;
	writeDescriptorSet[1].pTexelBufferView = 0;

	vkUpdateDescriptorSets(*device, 2, writeDescriptorSet, 0, 0); //causes access violation
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
