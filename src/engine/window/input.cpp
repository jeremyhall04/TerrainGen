#include "input.h"

GLFWwindow* Input::context = nullptr;

// returns TRUE if GLFW_PRESS or GLFW_REPEAT, and FALSE otherwise
bool Input::isKeyPressed(GLFWwindow* context, int keycode)
{
	int state = glfwGetKey(context, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

float Input::isMouseButtonPressed(GLFWwindow* context, int button)
{
	int state = glfwGetMouseButton(context, button);
	return state == GLFW_PRESS;
}

glm::vec2 Input::getMousePos(GLFWwindow* context)
{
	double x, y;
	glfwGetCursorPos(context, &x, &y);
	return glm::vec2(x, y);
}

float Input::getMouseX(GLFWwindow* context)
{
	glm::vec2 pos;
	pos = getMousePos(context);
	return pos.x;
}

float Input::getMouseY(GLFWwindow* context)
{
	glm::vec2 pos;
	pos = getMousePos(context);
	return pos.y;
}
