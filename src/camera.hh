#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct CameraUBO {
	glm::vec4 eye;
	glm::vec4 target;
	glm::vec4 up;
	glm::mat4 view;
	glm::mat4 camera;
};

class Camera
{
public:
	Camera(glm::vec4 eye, glm::vec4 target, glm::vec4 up, float fov, float ratio, float near, float far, float kspeed, float mspeed);
	~Camera();

	void process_mouse(float dx, float dy, float dt);
	void process_key(int key, float dt);

	void set_kspeed(float speed);
	void set_mspeed(float speed);

	glm::vec4 eye();
	glm::vec4 target();
	glm::vec4 up();
	glm::mat4 view();

	float kspeed();
	float mspeed();

private:
	glm::vec4 m_kspeed;
	float m_mspeed;

	CameraUBO m_ubo;

	float m_hangle;
	float m_vangle;
};
