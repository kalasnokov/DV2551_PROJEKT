#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/common.hpp>
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
	glm::vec2 chunkID = glm::vec2(2, 2);
	glm::vec3 pos = glm::vec3(2.0f,2.0f,2.0f);
	glm::vec3 front = glm::vec3(0, 1.0, 0);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 right;
	float roll, yaw, pitch;
	int width, height;
	glm::vec2 lastPos;
	void updateView()
	{
		glm::mat4 matRoll = glm::mat4(1.0f);//identity matrix; 
		glm::mat4 matPitch = glm::mat4(1.0f);//identity matrix
		glm::mat4 matYaw = glm::mat4(1.0f);//identity matrix

		//roll, pitch and yaw are used to store our angles in our class
		matRoll = glm::rotate(matRoll, roll, glm::vec3(0.0f, 0.0f, 1.0f));
		matPitch = glm::rotate(matPitch, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
		matYaw = glm::rotate(matYaw, yaw, glm::vec3(0.0f, 1.0f, 0.0f));

		//order matters
		glm::mat4 rotate = matRoll * matPitch * matYaw;

		glm::mat4 translate = glm::mat4(1.0f);
		translate = glm::translate(translate, -front);

		vp.view = rotate * translate;
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
		lastPos = glm::vec2((double)width / 2.0, (double)height / 2.0);
	}
	glm::vec2 getCurrentChunk()
	{
		return chunkID;
	}
	glm::vec3 getPlayerPos()
	{
		return pos;
	}
	matrices *update()
	{
		double x = 0, y = 0;
		glfwGetCursorPos(window, &x, &y);
		glm::vec2 mouse_delta = glm::vec2(x, y) - lastPos;
		lastPos = glm::vec2(x, y);
		float cameraSpeed = 0.005f; // adjust accordingly
		yaw += cameraSpeed * mouse_delta.x;
		pitch += cameraSpeed * mouse_delta.y;
		float dx = 0, dz = 0;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			dz = 2.0;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			dz = -2.0;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			dx = -2.0;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			dx = 2.0;
		auto mat = vp.view;
		glm::vec3 forward(mat[0][2], mat[1][2], mat[2][2]);
		glm::vec3 strafe(mat[0][0], mat[1][0], mat[2][0]);
		front += (-dz * forward + dx * strafe) * cameraSpeed * 20.f;
		this->updateView();
		//glfwSetCursorPos(window, double(width) / 2.0, double(height) / 2.0);
		return &vp;
	}
	~player()
	{

	}
};

