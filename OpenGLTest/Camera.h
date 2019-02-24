#pragma once
#include<glm/gtx/transform.hpp>
#include<GLFW/glfw3.h>
namespace Camera
{
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::vec3(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::mat4 view;
	glm::mat4 projection;
	int width = 800;
	int height = 600;

	float radius = 1.0f;
	void InitCamera()
	{
		
		cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		cameraUp = glm::cross(cameraDirection, cameraRight);
		view = glm::lookAt(cameraPos, cameraTarget, up);
		projection = glm::perspective(glm::radians(55.0f), (float)width / height, 0.1f, 100.0f);

	}
	void updateCamera(float phi, float theta)
	{
		float camX = radius * sin(phi) * sin(theta);
		float camY = radius * cos(theta);
		float camZ = radius * cos(phi) * sin(theta);

		cameraPos = glm::vec3(camX, camY, camZ);
		view = glm::lookAt(cameraPos,cameraTarget,up);
	}
}