#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class Camera
{
public:
	Camera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, float speed);
	~Camera();
	
	void process_key(int key, float dt);
	
	void set_speed(float speed);
	
	glm::vec3 eye();
	glm::vec3 target();
	glm::vec3 up();
	float speed();
	
private:
	glm::vec3 m_eye;
	glm::vec3 m_target;
	glm::vec3 m_up;
	glm::vec3 m_speed;
};
