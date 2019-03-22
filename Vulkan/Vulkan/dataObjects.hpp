#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
struct dataObjects {
	VkInstance instance;

	VkDevice device;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	VkQueue graphicsQueue;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;


	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	VkDebugUtilsMessengerEXT debugMessenger;

};