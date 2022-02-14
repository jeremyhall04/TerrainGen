#include "cameracontroller.h"
#include "camera.h"
#include "../window/input.h"

void OrthoCameraController::processInput(GLFWwindow* context, float deltaTime)
{
	glm::vec3 newPos = g_Camera2d->getPosition();
	float velocity = m_2DCameraSpeed * deltaTime;

	if (Input::isKeyPressed(context, GLFW_KEY_A))
		newPos.x -= velocity;
	if (Input::isKeyPressed(context, GLFW_KEY_D))
		newPos.x += velocity;
	if (Input::isKeyPressed(context, GLFW_KEY_W))
		newPos.y += velocity;
	if (Input::isKeyPressed(context, GLFW_KEY_S))
		newPos.y -= velocity;

	g_Camera2d->setPosition(newPos);
}

void PerspectiveCameraController::processInput(GLFWwindow* context, float deltaTime)
{
	glm::vec3 newPos = g_Camera3d->getPosition();
	glm::vec3 dir = g_Camera3d->getDirection();
	glm::vec3 right = g_Camera3d->getRight();
	glm::vec3 up = g_Camera3d->getUp();
	float velocity = m_3DCameraSpeed * deltaTime;

	if (Input::isKeyPressed(context, GLFW_KEY_W))
		newPos += glm::vec3((dir.x + up.x) * velocity, 0.0f, (dir.z + up.z) * velocity);
	if (Input::isKeyPressed(context, GLFW_KEY_S))
		newPos -= glm::vec3((dir.x + up.x) * velocity, 0.0f, (dir.z + up.z) * velocity);
	if (Input::isKeyPressed(context, GLFW_KEY_A))
		newPos -= glm::vec3(right.x * velocity, 0.0f, right.z * velocity);
	if (Input::isKeyPressed(context, GLFW_KEY_D))
		newPos += glm::vec3(right.x * velocity, 0.0f, right.z * velocity);
	if (Input::isKeyPressed(context, GLFW_KEY_SPACE))
		newPos.y += velocity;
	if (Input::isKeyPressed(context, GLFW_KEY_LEFT_SHIFT))
		newPos.y -= velocity;

	g_Camera3d->setPosition(newPos);
}
