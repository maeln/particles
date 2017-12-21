#include "camera.hh"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/trigonometric.hpp>

Camera::Camera(glm::vec4 eye, glm::vec4 target, glm::vec4 up, float fov, float ratio, float near, float far, float kspeed, float mspeed)
{
	m_kspeed = glm::vec4(kspeed, kspeed, kspeed, 1.f);
	m_mspeed = mspeed;

	m_hangle = 0.f;
	m_vangle = 0.f;

	m_ubo = {
		eye,
		target,
		up,
		glm::lookAt(glm::vec3(eye), glm::vec3(eye+target), glm::vec3(up)),
		glm::perspective(glm::radians(60.f), ratio, near, far)
	};
}

Camera::~Camera()
{
}

void Camera::process_mouse(float dx, float dy, float dt)
{
	m_hangle += dx*m_mspeed;
	m_vangle += dy*m_mspeed;

	m_ubo.target = glm::vec4(
	               glm::cos(m_vangle)*glm::sin(m_hangle),
	               glm::sin(m_vangle),
	               glm::cos(m_vangle)*glm::cos(m_hangle),
				   1.f
	           );

	glm::vec3 right = glm::vec3(
	                      glm::sin(m_hangle - 3.14f/2.f),
	                      0,
	                      glm::cos(m_hangle - 3.14f/2.f)
	                  );

	m_ubo.up = glm::vec4(glm::cross(right, glm::vec3(m_ubo.target)), 1.f);

	m_ubo.view = glm::lookAt(glm::vec3(m_ubo.eye), glm::vec3(m_ubo.eye+m_ubo.target), glm::vec3(m_ubo.up));
}

void Camera::process_key(int key, float dt)
{
	if(key == GLFW_KEY_W)
	{
		m_ubo.eye += (m_ubo.target * m_kspeed * glm::vec4(dt, dt, dt, 1.f));
	}

	if(key == GLFW_KEY_S)
	{
		m_ubo.eye -= (m_ubo.target * m_kspeed * glm::vec4(dt, dt, dt, 1.f));
	}

	if(key == GLFW_KEY_A)
	{
		glm::vec3 left = glm::cross(glm::vec3(m_ubo.up), glm::vec3(m_ubo.target));
		left = glm::normalize(left);
		m_ubo.eye += (glm::vec4(left, 1.f) * m_kspeed * glm::vec4(dt, dt, dt, 1.f));
	}

	if(key == GLFW_KEY_D)
	{
		glm::vec3 left = glm::cross(glm::vec3(m_ubo.target), glm::vec3(m_ubo.up));
		left = glm::normalize(left);
		m_ubo.eye += (glm::vec4(left, 1.f) * m_kspeed * glm::vec4(dt, dt, dt, 1.f));
	}

	if(key == GLFW_KEY_SPACE)
	{
		m_ubo.eye += (glm::vec4(0.f, 1.f, 0.f, 0.f) * m_kspeed * glm::vec4(dt, dt, dt, 1.f));
	}

	if(key == GLFW_KEY_LEFT_SHIFT)
	{
		m_ubo.eye -= (glm::vec4(0.f, 1.f, 0.f, 0.f) * m_kspeed * glm::vec4(dt, dt, dt, 1.f));
	}

	m_ubo.view = glm::lookAt(glm::vec3(m_ubo.eye), glm::vec3(m_ubo.eye+m_ubo.target), glm::vec3(m_ubo.up));
}

void Camera::set_kspeed(float speed)
{
	m_kspeed = glm::vec4(speed);
}

void Camera::set_mspeed(float speed)
{
	m_mspeed = speed;
}

glm::vec4 Camera::eye()
{
	return m_ubo.eye;
}

glm::vec4 Camera::target()
{
	return m_ubo.target;
}

glm::vec4 Camera::up()
{
	return m_ubo.up;
}

glm::mat4 Camera::view()
{
	return m_ubo.view;
}

float Camera::kspeed()
{
	return m_kspeed.x;
}

float Camera::mspeed()
{
	return m_mspeed;
}
