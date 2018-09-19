#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
	Camera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, float kspeed, float mspeed);
	~Camera();

	void process_mouse(float dx, float dy, float dt);
	void process_key(int key, float dt);

	void set_kspeed(float speed);
	void set_mspeed(float speed);

	glm::vec3 eye();
	glm::vec3 target();
	glm::vec3 up();

	glm::mat4 view();

	float kspeed();
	float mspeed();

private:
	glm::vec3 m_eye;
	glm::vec3 m_target;
	glm::vec3 m_up;
	glm::vec3 m_kspeed;
	float m_mspeed;
	glm::mat4 m_view;

	float m_hangle;
	float m_vangle;
};
