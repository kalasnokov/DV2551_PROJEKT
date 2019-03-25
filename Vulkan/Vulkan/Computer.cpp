#include "pch.h"
#include "Computer.h"


Computer::Computer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkQueue* queue, std::string shaderLoc, std::vector<uint32_t> bufferSizes){
	this->device = device;
	this->physicalDevice = physicalDevice;
	this->queue = queue;

	for (int i = 0; i < bufferSizes.size(); i++) {
		bufferInfo info;
		info.bufferSize = bufferSizes.at(i);
		bufferStructs.push_back(info);
	}

	std::cout << bufferStructs.size() << " buffers are to be created\n";

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

void Computer::populateBuffer(int loc, const void* src) {
	void* data;
	vkMapMemory(*device, bufferStructs.at(loc).bufferMem, 0, bufferStructs.at(loc).bufferSize, 0, &data);
	memcpy(data, src, (size_t)bufferStructs.at(loc).bufferSize);
	vkUnmapMemory(*device, bufferStructs.at(loc).bufferMem);
}

void* Computer::readBuffer(int loc) {
	void* data;
	vkMapMemory(*device, bufferStructs.at(loc).bufferMem, 0, bufferStructs.at(loc).bufferSize, 0, &data);
	vkUnmapMemory(*device, bufferStructs.at(loc).bufferMem);
	return data;
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
	for (int i = 0; i < bufferStructs.size(); i++) {
		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = bufferStructs.at(i).bufferSize;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VHF::createBuffer(*device, *physicalDevice, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferStructs.at(i).buffer, bufferStructs.at(i).bufferMem, bufferCreateInfo);
	}
}


void Computer::commandBufferSetup() {
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = 0;
	commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

	if (vkCreateCommandPool(*device, &commandPoolCreateInfo, 0, &commandPool) != VK_SUCCESS) {
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
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSet, 0, 0);

		vkCmdDispatch(commandBuffer, 5, 1, 1);//needs more workers?

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO END COMMAND BUFFER!");
	}
}

void Computer::descriptorLayoutSetup() {
	VkDescriptorSetLayoutBinding* descriptorSetLayoutBinding = new VkDescriptorSetLayoutBinding[bufferStructs.size()];
	for (int i = 0; i < bufferStructs.size(); i++) {
		descriptorSetLayoutBinding[i].binding = i;
		descriptorSetLayoutBinding[i].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorSetLayoutBinding[i].descriptorCount = 1;
		descriptorSetLayoutBinding[i].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
		descriptorSetLayoutBinding[i].pImmutableSamplers = 0;
	}

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.bindingCount = bufferStructs.size();
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBinding;
	descriptorSetLayoutCreateInfo.flags = 0;
	descriptorSetLayoutCreateInfo.pNext = 0;

	// Create the descriptor set layout. 
	if (vkCreateDescriptorSetLayout(*device, &descriptorSetLayoutCreateInfo, 0, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("COMPUTER ERROR: FAILED TO SETUP DESCRIPTORLAYOUT!");
	}

	delete[] descriptorSetLayoutBinding;
}

void Computer::descriptorSetup() {
	VkDescriptorPoolSize descriptorPoolSize = {};
	descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorPoolSize.descriptorCount = bufferStructs.size();

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

	VkWriteDescriptorSet* writeDescriptorSets = new VkWriteDescriptorSet[bufferStructs.size()];
	VkDescriptorBufferInfo* infos = new VkDescriptorBufferInfo[bufferStructs.size()];
	for (int i = 0; i < bufferStructs.size(); i++) {
		infos[i].buffer = bufferStructs.at(i).buffer;
		infos[i].offset = 0;
		infos[i].range = bufferStructs.at(i).bufferSize;

		writeDescriptorSets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[i].pNext = 0;
		writeDescriptorSets[i].dstSet = descriptorSet;
		writeDescriptorSets[i].dstBinding = i;
		writeDescriptorSets[i].dstArrayElement = 0;
		writeDescriptorSets[i].descriptorCount = 1;
		writeDescriptorSets[i].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		writeDescriptorSets[i].pImageInfo = 0;
		writeDescriptorSets[i].pBufferInfo = &infos[i];
		writeDescriptorSets[i].pTexelBufferView = 0;
	}

	vkUpdateDescriptorSets(*device, bufferStructs.size(), writeDescriptorSets, 0, 0);

	delete[] writeDescriptorSets;
	delete[] infos;
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
	int32_t res = vkCreateComputePipelines(*device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, NULL, &pipeline);
	if (res != VK_SUCCESS) {
		std::cout << "ERROR CODE: " << res << "!\n";
		throw std::runtime_error("COMPUTER ERROR: FAILED TO CREATE PIPELINE!");
	}
}

void Computer::loadShader(std::string shaderLoc) {
	auto code = VHF::readFile(shaderLoc);
	shader = VHF::createShaderModule(*device, code);
}

Computer::~Computer() {

}
