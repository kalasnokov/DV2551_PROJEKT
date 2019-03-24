#pragma once
#include <iostream>
#ifdef _WIN64
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
#endif
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>

#include <iostream>
#include <vector>
#include <optional>
#include <set>
#include <fstream>
#include <array>

#include "dataObjects.hpp"

namespace VHF {//Vulkan Helper Functions
	uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);

	std::vector<char> readFile(const std::string& filename);

	void createBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkBufferCreateInfo bufferCreateInfo);

	void cpyBuf(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer src, VkBuffer dst, VkDeviceSize size);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

	bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

	bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

	uint32_t findQueueFamily(VkPhysicalDevice physicalDevice, VkQueueFlagBits queueType);
}