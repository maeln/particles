#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/trigonometric.hpp>

Camera::Camera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, float kspeed, float mspeed)
{
	m_eye = eye;
	m_target = target;
	m_up = up;
	m_kspeed = glm::vec3(kspeed);
	m_mspeed = mspeed;
	
	m_hangle = 0.f;
	m_vangle = 0.f;
	
	m_view = glm::lookAt(m_eye, m_eye+m_target, m_up);
}

Camera::~Camera()
{
}

void Camera::process_mouse(float dx, float dy, float dt)
{
	m_hangle += dx*m_mspeed*dt;
	m_vangle += dy*m_mspeed*dt;
	
	m_target = glm::vec3(
		glm::cos(m_vangle)*glm::sin(m_hangle),
		glm::sin(m_vangle),
		glm::cos(m_vangle)*glm::cos(m_hangle)
	);
	
	glm::vec3 right = glm::vec3(
		glm::sin(m_hangle - 3.14f/2.f),
		0,
		glm::cos(m_hangle - 3.14f/2.f)
	);
	
	m_up = glm::cross(right, m_target);
	
	m_view = glm::lookAt(m_eye, m_eye+m_target, m_up);
}

void Camera::process_key(int key, float dt)
{
	if(key == GLFW_KEY_W)
	{
		m_eye += (m_target * m_kspeed * glm::vec3(dt));
	}
		
	if(key == GLFW_KEY_S)
	{
		m_eye -= (m_target * m_kspeed * glm::vec3(dt));
	}
		
	if(key == GLFW_KEY_A)
	{
		glm::vec3 left = glm::cross(m_up, m_target);
		left = glm::normalize(left);
		m_eye += (left * m_kspeed * glm::vec3(dt));
	}
		
	if(key == GLFW_KEY_D)
	{
		glm::vec3 left = glm::cross(m_target, m_up);
		left = glm::normalize(left);
		m_eye += (left * m_kspeed * glm::vec3(dt));
	}
	
	if(key == GLFW_KEY_SPACE)
	{
		m_eye += (glm::vec3(0.f, 1.f, 0.f) * m_kspeed * glm::vec3(dt));
	}
	
	if(key == GLFW_KEY_LEFT_SHIFT)
	{
		m_eye -= (glm::vec3(0.f, 1.f, 0.f) * m_kspeed * glm::vec3(dt));
	}
	
	m_view = glm::lookAt(m_eye, m_eye+m_target, m_up);
}
	
void Camera::set_kspeed(float speed)
{
	m_kspeed = glm::vec3(speed);
}

void Camera::set_mspeed(float speed)
{
	m_mspeed = speed;
}
	
glm::vec3 Camera::eye()
{
	return m_eye;
}

glm::vec3 Camera::target()
{
	return m_target;
}

glm::vec3 Camera::up()
{
	return m_up;
}

glm::mat4 Camera::view()
{
	return m_view;
}

float Camera::kspeed()
{
	return m_kspeed.x;
}

float Camera::mspeed()
{
	return m_mspeed;
}
