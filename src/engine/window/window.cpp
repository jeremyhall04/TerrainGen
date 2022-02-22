#include "window.h"
#include "input.h"
#include "../utils/camera.h"
#include "../utils/cameracontroller.h"
#include "../utils/timer.h"
#include <iostream>

bool isWireframe = false;
Timer time;
float timer = 0.0f;
UINT frames = 0;
float deltaTime = 0.0f, lastFrame = 0.0f;

Window::Window()
{
	Title = "OpenGL Application";
	init();
}

Window::Window(const char* windowTitle)
{
	Title = windowTitle;
	init();
}

Window::~Window()
{
}

bool Window::isRunning()
{
	return !glfwWindowShouldClose(context);
}

void Window::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	context = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, Title, NULL, NULL);
	if (context == NULL)
	{
		std::cout << "ERROR::GLFW::WINDOW CREATION FAILED\n" << std::endl;
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(context);
	glfwSetFramebufferSizeCallback(context, framebuffer_size_callback);
	glfwSetCursorPosCallback(context, mouse_callback);
	glfwSetInputMode(context, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetMouseButtonCallback(handle, mouse_button_callback);

	glfwSwapInterval(0); // 0 for uncapped fps

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD::GLAD INITIALIZATION FAILED\n" << std::endl;
		exit(1);
	}
}

void Window::update()
{
	static unsigned int avg;

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		std::cout << "\nERROR::WINDOW::UPDATE::OpenGl error::" << error;

	g_Camera2d->update();
	g_Camera3d->update();

	if (isWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glfwSwapBuffers(context);
	glfwPollEvents();

	frames++;
	if (time.elapsed() - timer > 1.0f)
	{
		timer += 1.0f;
		avg += frames;
		printf("\n%d fps, avg = %f", frames, avg / timer);
		frames = 0;
	}
}

void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::processInput()
{
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	
	if (Input::isKeyPressed(context, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(context, true);

	OrthoCameraController::processInput(context, deltaTime);
	PerspectiveCameraController::processInput(context, deltaTime);
	// keyboard input
	//if (glfwGetKey(context, GLFW_KEY_W) == GLFW_PRESS)
	//	g_CameraPtr->processKeyboardInput(CameraMovement::FORWARD, deltaTime);
	//if (glfwGetKey(context, GLFW_KEY_S) == GLFW_PRESS)
	//	g_CameraPtr->processKeyboardInput(CameraMovement::BACKWARD, deltaTime);
	//if (glfwGetKey(context, GLFW_KEY_A) == GLFW_PRESS)
	//	g_CameraPtr->processKeyboardInput(CameraMovement::LEFT, deltaTime);
	//if (glfwGetKey(context, GLFW_KEY_D) == GLFW_PRESS)
	//	g_CameraPtr->processKeyboardInput(CameraMovement::RIGHT, deltaTime);
	//if (glfwGetKey(context, GLFW_KEY_SPACE) == GLFW_PRESS)
	//	g_CameraPtr->processKeyboardInput(CameraMovement::UP, deltaTime);
	//if (glfwGetKey(context, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	//	g_CameraPtr->processKeyboardInput(CameraMovement::DOWN, deltaTime);

	static bool wasPressed;
	if (Input::isKeyPressed(context, GLFW_KEY_O) && !wasPressed)
	{
		isWireframe = !isWireframe;
		wasPressed = true;
	}
	else if (!Input::isKeyPressed(context, GLFW_KEY_O))
		wasPressed = false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (m_FirstMouse)
	{
		m_MouseLastX = (float)SCR_WIDTH / 2.0f;
		m_MouseLastY = (float)SCR_HEIGHT / 2.0f;
		glfwSetCursorPos(window, m_MouseLastX, m_MouseLastY);
		m_FirstMouse = false;
	}

	float xOffset = (float)xpos - m_MouseLastX;
	float yOffset = m_MouseLastY - (float)ypos;

	m_MouseLastX = (float)xpos;
	m_MouseLastY = (float)ypos;
	xOffset *= m_MouseSensitivity;
	yOffset *= m_MouseSensitivity;

	g_Camera3d->processMouseMovement(xOffset, yOffset);
}