#include "camera.h"
#include "../window/window.h"

extern OrthographicCamera*	g_Camera2d = 0;
extern PerspectiveCamera*	g_Camera3d = 0;

//OrthographicCamera::OrthographicCamera()
//{
//    projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
//    view = glm::mat4(1.0);
//    viewProjection = projection * view;
//}
//
//OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
//{
//    projection = glm::ortho(left, right, bottom, top, -10.0f, 10.0f);
//    view = glm::mat4(1.0);
//    viewProjection = projection * view;
//}
//
//OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
//{
//    projection = glm::ortho(left, right, bottom, top, zNear, zFar);
//    view = glm::mat4(1.0);
//    viewProjection = projection * view;
//}
//
//OrthographicCamera::~OrthographicCamera()
//{
//}
//
//void OrthographicCamera::recalculateViewMatrix()
//{
//    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
//
//    view = glm::inverse(transform);
//    viewProjection = projection * view;
//}
//
//
//
//
//
//
//
//
//
//PerspectiveCamera::PerspectiveCamera()
//{
//    projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
//    view = glm::mat4(1.0);
//    viewProjection = projection * view;
//}
//
//PerspectiveCamera::PerspectiveCamera(float left, float right, float bottom, float top)
//{
//    projection = glm::ortho(left, right, bottom, top, -10.0f, 10.0f);
//    view = glm::mat4(1.0);
//    viewProjection = projection * view;
//}
//
//PerspectiveCamera::PerspectiveCamera(float left, float right, float bottom, float top, float zNear, float zFar)
//{
//    projection = glm::ortho(left, right, bottom, top, zNear, zFar);
//    view = glm::mat4(1.0);
//    viewProjection = projection * view;
//}
//
//PerspectiveCamera::~PerspectiveCamera()
//{
//}
//
//void PerspectiveCamera::recalculateViewMatrix()
//{
//    glm::vec3 dir;
//    float pitchR = glm::radians(m_Pitch);
//    float yawR = glm::radians(m_Yaw);
//
//    dir.x = cos(yawR) * cos(pitchR);
//    dir.y = sin(pitchR);
//    dir.z = sin(yawR) * cos(pitchR);
//
//    direction = glm::normalize(dir);
//    right = glm::normalize(glm::cross(direction, worldUp));
//    up = glm::normalize(glm::cross(right, direction));
//
//    view = glm::lookAt(position, position + direction, up);
//    projection = glm::perspective(m_FOV, m_Aspect, m_zNear, m_zFar);
//    viewProjection = projection * view;
//}


Camera::Camera()
{}

Camera::Camera(glm::vec3 position, glm::vec3 direction)
	: m_Position(position), m_Direction(direction)
{}


Camera::~Camera()
{}





OrthographicCamera::OrthographicCamera()
	: Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1)), m_HorLimits(glm::vec2(-1.0f, 1.0f)), m_VertLimits(glm::vec2(-1.0f, 1.0f))
{
	update();
}

OrthographicCamera::OrthographicCamera(glm::vec3 position, glm::vec3 direction, glm::vec2 h_limits, glm::vec2 v_limits)
	: Camera(position, direction), m_HorLimits(h_limits), m_VertLimits(v_limits)
{
	update();
}

void OrthographicCamera::update()
{
	view = glm::mat4(1.0f);
	projection = glm::ortho(m_HorLimits[0], m_HorLimits[1], m_VertLimits[0], m_VertLimits[1], -100.0f, 100.0f);
}




PerspectiveCamera::PerspectiveCamera()
{}

PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 direction, float fov)
	: Camera(position, direction), m_FOV(fov), m_Aspect((float)SCR_WIDTH / (float)SCR_HEIGHT), m_zNear(0.01f), m_zFar(1000.0f)
{
	m_Pitch = direction.y;
	m_Yaw = (atan2(direction.z, direction.x) * 180) / PI;
	update();
}

void PerspectiveCamera::processMouseMovement(float xOffset, float yOffset)
{
	m_Yaw += xOffset;
	m_Pitch += yOffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;
}

void PerspectiveCamera::update()
{
	glm::vec3 dir;
	float pitchR = glm::radians(m_Pitch);
	float yawR = glm::radians(m_Yaw);
	
	dir.x = cos(yawR) * cos(pitchR);
	dir.y = sin(pitchR);
	dir.z = sin(yawR) * cos(pitchR);
	//printf("\ndir = <%f, %f, %f>", dir.x, dir.y, dir.z);

	m_Direction = glm::normalize(dir);
	m_Right = glm::normalize(glm::cross(m_Direction, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Direction));

	view = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
	projection = glm::perspective(m_FOV, m_Aspect, m_zNear, m_zFar);
}

void PerspectiveCamera::updateAspect(float aspect)
{
	m_Aspect = aspect;
}
