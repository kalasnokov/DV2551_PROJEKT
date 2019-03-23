#include "pch.h"
#include "Computer.h"


Computer::Computer(VkDevice* device, VkPhysicalDevice* physicalDevice, std::string shaderLoc){
	this->device = device;
	this->physicalDevice = physicalDevice;

	loadShader(shaderLoc);
	descriptorSetup();
	std::cout << "COMPUTER: DESCRIPTORS SET!\n";
	pipelineSetup();
	std::cout << "COMPUTER: PIPELINE SET!\n";
}

void Computer::commandBufferSetup() {

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
}

void Computer::loadShader(std::string shaderLoc) {
	auto code = VHF::readFile(shaderLoc);
	shader = VHF::createShaderModule(*device, code);
}

Computer::~Computer() {

}
