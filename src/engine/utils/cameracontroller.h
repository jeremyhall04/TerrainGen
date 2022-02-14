#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "../glHeader.h"

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class OrthoCameraController
{
public:
	static void processInput(GLFWwindow* context, float deltaTime);
};

class PerspectiveCameraController
{
public:
	static void processInput(GLFWwindow* context, float deltaTime);
};

#endif // !CAMERA_CONTROLLER_H
