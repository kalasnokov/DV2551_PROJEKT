#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const float SENSITIVITY = 0.01f;
struct matrices
{
	glm::mat4 view;
	glm::mat4 projection;
};
class player
{
private:
	GLFWwindow *window = nullptr;
	int chunkID;
	glm::vec3 pos = glm::vec3(2.0f,2.0f,2.0f);
	glm::vec3 front = glm::vec3(0, 1.0, 0);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 right;
	float Yaw = -90.0f;
	float Pitch;
	int width, height;
	bool firstMouse = true;
	double lastX, lastY;
	void updateCameraVectors()
	{
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, up)); 
	}
	void ProcessMouseMovement()
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		auto xoffset = xpos - lastX;
		auto yoffset = ypos - lastY;
		xoffset *= SENSITIVITY;
		yoffset *= SENSITIVITY;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
		updateCameraVectors();
		glfwSetCursorPos(window, (double)width / 2.0, (double)height / 2.0);
		lastX = (double)width / 2.0;
		lastY = (double)height / 2.0;
	}
public:
	matrices vp;
	player(){}
	void init(GLFWwindow *window)
	{
		this->window = window;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwGetWindowSize(window, &width, &height);
		vp.projection = glm::perspective(glm::radians(80.0f), (float)width / (float)height, 0.1f, 100.0f);
		lastX = (double)width / 2.0;
		lastY = (double)height / 2.0;
	}
	matrices *update()
	{
		float cameraSpeed = 0.05f; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			pos += cameraSpeed * front;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			pos -= cameraSpeed * front;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			pos -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			pos += glm::normalize(glm::cross(front, up)) * cameraSpeed;
		//this->ProcessMouseMovement();
		vp.view = glm::lookAt(pos, pos + front, up);
		return &vp;
	}
	~player()
	{

	}
};

