#ifndef CAMERA_H
#define CAMERA_H

#include "../glHeader.h"

const float m_CameraSpeed = 5.0f;
const float m_MouseSensitivity = 0.1f;
static bool m_FirstMouse = true;
static float m_MouseLastX, m_MouseLastY;

const float m_2DCameraSpeed = 4.0f;
const float m_3DCameraSpeed = 50.0f;

//class OrthographicCamera
//{
//private:
//	glm::mat4 view;
//	glm::mat4 projection;
//	glm::mat4 viewProjection;
//	glm::vec3 position;
//
//public:
//	OrthographicCamera();
//	OrthographicCamera(float left, float right, float bottom, float top);
//	OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar);
//	~OrthographicCamera();
//
//	void setPosition(const glm::vec3& pos)
//	{
//		this->position = pos;
//		recalculateViewMatrix();
//	};
//	const glm::vec3& getPosition() { return position; };
//
//	const glm::mat4& getViewMatrix() { return view; };
//	const glm::mat4& getProjectionMatrix() { return projection; };
//	const glm::mat4& getViewProjectionMatrix() { return viewProjection; };
//
//private:
//	void recalculateViewMatrix();
//};
//
//class PerspectiveCamera
//{
//private:
//	glm::mat4 view, projection, viewProjection;
//	glm::vec3 position, direction, right, up, worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
//	float m_FOV, m_Aspect, m_zNear, m_zFar;
//	float m_Pitch, m_Yaw;
//
//public:
//	PerspectiveCamera();
//	PerspectiveCamera(float left, float right, float bottom, float top);
//	PerspectiveCamera(float left, float right, float bottom, float top, float zNear, float zFar);
//	~PerspectiveCamera();
//
//	void setPosition(const glm::vec3& pos)
//	{
//		this->position = pos;
//		recalculateViewMatrix();
//	};
//	const glm::vec3& getPosition() { return position; };
//	const glm::vec3& getDirection() { return direction; };
//	const glm::vec3& getRight() { return right; };
//	const glm::vec3& getUp() { return up; };
//
//	const glm::mat4& getViewMatrix() { return view; };
//	const glm::mat4& getProjectionMatrix() { return projection; };
//	const glm::mat4& getViewProjectionMatrix() { return viewProjection; };
//
//private:
//	void recalculateViewMatrix();
//};


//-----------------------------------------------------------------------------------------------------
//-------------------------------------------- CAMERA -------------------------------------------------
//-----------------------------------------------------------------------------------------------------
class Camera
{
protected:
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 viewprojection;
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	const glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 direction);
	~Camera();

	//glm::mat4 getViewMatrix() { return glm::lookAt(m_Position, m_Position + m_Direction, m_Up); };
	virtual void update() {}
	void setPosition(const glm::vec3& pos) { m_Position = pos; };
	glm::mat4 getViewMatrix() { return view; };
	glm::mat4 getProjectionMatrix() { return projection; };
	const glm::vec3& getPosition() { return m_Position; };
	const glm::vec3& getDirection() { return m_Direction; };
	const glm::vec3& getRight() { return m_Right; };
	const glm::vec3& getUp() { return m_Up; };
};

//-----------------------------------------------------------------------------------------------------
//------------------------------------------- Orthographic --------------------------------------------
//-----------------------------------------------------------------------------------------------------
class OrthographicCamera : public Camera
{
private:
	glm::vec2 m_HorLimits, m_VertLimits;
public:
	OrthographicCamera();
	OrthographicCamera(glm::vec3 position, glm::vec3 direction, glm::vec2 hlimits, glm::vec2 vlimits);
	//OrthoCamera(float xPos, float yPos, float zPos, glm::vec3 direction, glm::vec2 hlimits, glm::vec2 vlimits);

	void update() override;
};

//-----------------------------------------------------------------------------------------------------
//------------------------------------------- Perspective ---------------------------------------------
//-----------------------------------------------------------------------------------------------------
class PerspectiveCamera : public Camera
{
private:
	float m_FOV, m_Aspect, m_zNear, m_zFar;
	float m_Pitch, m_Yaw;
public:
	PerspectiveCamera();
	PerspectiveCamera(glm::vec3 position, glm::vec3 direction, float fov);
	//PerspectiveCamera(float xPos, float yPos, float zPos, glm::vec3 direction, float fov);

	//void processKeyboardInput(GLFWwindow* window, float deltaTime);
	void processMouseMovement(float xOffset, float yOffset);
	void update() override;
	const glm::vec4& getPerspectiveParams() const { return glm::vec4(m_FOV, m_Aspect, m_zNear, m_zFar); };
	void updateAspect(float aspect);
};


extern OrthographicCamera* g_Camera2d;
extern PerspectiveCamera* g_Camera3d;

#endif // !CAMERA_H