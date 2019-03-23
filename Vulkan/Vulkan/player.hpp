#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class player
{
private:
	GLFWwindow *window = nullptr;
	int chunkID;
	glm::vec3 pos;
	glm::vec3 viewpos;
	struct matrices
	{
		glm::mat4 view;
		glm::mat4 projection;
	}vp;
	int width, height;
public:
	player(GLFWwindow *window)
	{
		this->window = window;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwGetWindowSize(window, &width, &height);
		vp.projection = glm::perspective(glm::radians(80.0f), (float)width / (float)height, 0.1f, 100.0f);
	}
	matrices *update()
	{
		vp.view = glm::lookAt(pos, viewpos, glm::vec3(0, 0, 1));
		return &vp;
	}
	~player();
};

