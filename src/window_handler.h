#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <map>
#include <string>
#include <memory>

#include "particle_handler.h"
#include "shader_handler.h"
#include "camera.h"

class WindowHandler
{
public:
	static WindowHandler& instance()
	{
		static WindowHandler instance;
		return instance;
	}
	~WindowHandler();
	
	void setup();
	void rendering_loop();
	
private:
	WindowHandler();
	WindowHandler(WindowHandler const&);
	void operator=(WindowHandler const&);
	
	static void fw_resize_callback(GLFWwindow* window, int width, int height) { instance().resize_callback(window, width, height); };
	static void fw_error_callback(int error, const char* description) { instance().error_callback(error, description); };
	static void fw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) { instance().keyboard_callback(window, key, scancode, action, mods); };
	static void fw_mouse_callback(GLFWwindow* window, double xpos, double ypos) { instance().mouse_callback(window, xpos, ypos); };
	
	void resize_callback(GLFWwindow* window, int width, int height);
	void error_callback(int error, const char* description);
	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	
	// Private vars.
	GLFWwindow* m_window;
	GLuint m_vao;
	GLuint m_width;
	GLuint m_height;
	bool m_vsync;
	
	GLuint mpp, mpd, mpi;
	GLint pers, cam;
	
	double m_mouse_dx;
	double m_mouse_dy;
	
	double m_frame_dt;
	double m_prev_t;
		
	std::unique_ptr<Camera> m_camera;
	
	glm::mat4 m_perpective_matrix;
	
	GLuint m_max_part;
	std::unique_ptr<ParticleHandler> m_particles;
	ShaderHandler& m_shaders = ShaderHandler::instance();
	std::map<std::string, std::shared_ptr<Shader>> m_shader_cache; //TODO: Delete pointer or use shared_ptr
	std::map<std::string, std::shared_ptr<Program>> m_programs; //TODO: Delete pointer or use shared_ptr
};
