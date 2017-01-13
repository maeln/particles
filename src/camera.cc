#include "camera.h"

Camera::Camera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, float kspeed, mspeed)
{
	m_eye = eye;
	m_target = target;
	m_up = up;
	m_kspeed = glm::vec3(kspeed);
	m_mspeed = glm::vec3(mspeed);
}

Camera::~Camera()
{
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
}
	
void Camera::set_kspeed(float speed)
{
	m_kspeed = glm::vec3(speed);
}

void Camera::set_mspeed(float speed)
{
	m_mspeed = glm::vec3(speed);
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

float Camera::kspeed()
{
	return m_kspeed.x;
}

float Camera::mspeed()
{
	return m_mspeed.x;
}
