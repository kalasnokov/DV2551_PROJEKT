#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include <optional>
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

struct computeObjects {
	VkBuffer data;
	VkBuffer uniforms;
	VkQueue queue;
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> computeFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value() && computeFamily.has_value();
	}
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};