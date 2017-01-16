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
	m_view = glm::lookAt(m_eye, m_target, m_up);
	
	glm::vec3 htarget(m_target.x, 0.f, m_target.z);
	htarget = glm::normalize(htarget);
	if(htarget.z >= 0.f)
	{
		if(htarget.x >= 0.f)
			m_hangle = 360.f - glm::degrees(glm::asin(htarget.z));
		else
			m_hangle = 180.f + glm::degrees(glm::asin(htarget.z));
	}
	else
	{
		if(htarget.x >= 0.f)
			m_hangle = glm::degrees(glm::asin(-htarget.z));
		else
			m_hangle = 180.f - glm::degrees(glm::asin(-htarget.z));
	}
	m_vangle = -glm::degrees(glm::asin(m_target.y));
}

Camera::~Camera()
{
}

void Camera::process_mouse(float dx, float dy, float dt)
{
	m_hangle += dx*m_mspeed*dt;
	m_vangle += dy*m_mspeed*dt;
	
	glm::vec3 v_axis = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 view = glm::vec3(1.f, 0.f, 0.f);
	view = glm::normalize(glm::rotate(view, m_hangle, v_axis));
	glm::vec3 h_axis = glm::normalize(glm::cross(v_axis, view));
	view = glm::normalize(glm::rotate(view, m_vangle, h_axis));
	m_target = view;
	m_up = glm::normalize(glm::cross(m_target, h_axis));
	
	m_view = glm::lookAt(m_eye, m_target, m_up);
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
	
	m_view = glm::lookAt(m_eye, m_target, m_up);
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
