#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>

class WindowHandler
{
public:
	WindowHandler(int width, int height);
	~WindowHandler();
	
	void setup();
	void rendering_loop();
	
private:
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void error_callback(int error, const char* description);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	
	// Private vars.
	GLFWwindow* m_window;
	GLuint m_width;
	GLuint m_height;
};
