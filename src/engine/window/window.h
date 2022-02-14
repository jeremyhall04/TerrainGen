#ifndef WINDOW_H
#define WINDOW_H

#include "../glHeader.h"

const int SCR_WIDTH = 960;
const int SCR_HEIGHT = 540;

class Window
{
private:
	const char* Title;
public:
	GLFWwindow* context;

public:
	Window();
	Window(const char* windowTitle);
	~Window();

private:
	void init();
public:
	bool isRunning();
	void update();
	void clear();
	void processInput();
	void getMousePos(double& x, double& y);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif

