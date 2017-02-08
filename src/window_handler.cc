#include "window_handler.hh"

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <exception>

#include "tools.hh"
#include "data.hh"
#include "indiced_mesh.hh"

WindowHandler::WindowHandler()
{
	m_width = 1280;
	m_height = 768;

	/* Initialize the library */
	if (!glfwInit())
		throw new std::runtime_error("Unable to launch GLFW.");
	glfwSetErrorCallback(&WindowHandler::fw_error_callback);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	/* Create a windowed mode window and its OpenGL context */
	m_window = glfwCreateWindow(m_width, m_height, "Hello World", NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		throw new std::runtime_error("Unable to display the window.");
	}

	// Setup callbacks;
	glfwSetFramebufferSizeCallback(m_window, &WindowHandler::fw_resize_callback);
	glfwSetKeyCallback(m_window, &WindowHandler::fw_keyboard_callback);
	glfwSetCursorPosCallback(m_window, &WindowHandler::fw_mouse_callback);

	// OpenGL context.
	glfwMakeContextCurrent(m_window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glViewport(0, 0, m_width, m_height);

	m_mouse_dx = 0.0;
	m_mouse_dy = 0.0;
	m_frame_dt = 0.0;
	m_prev_t = 0.0;

	m_max_part = 10000;
	m_particles = std::unique_ptr<ParticleHandler>(new ParticleHandler(m_max_part, 2.5, 1.0, glm::vec3(0.0, 0.5, 0.0), false, glm::vec3(41.0/255.0, 114.0/255.0, 200.0/255.0)));
	m_vsync = true;

	m_camera = std::unique_ptr<Camera>(new Camera(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f), 0.5, 0.01));
}

WindowHandler::~WindowHandler()
{
	m_shader_cache.clear();
	m_programs.clear();
	m_particles.reset();
	glfwTerminate();
}

void WindowHandler::setup()
{
	// OpenGL stuff
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.f, 1.f);
	glEnable(GL_DEPTH_CLAMP);

	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// VSync.
	glfwSwapInterval(m_vsync ? 1 : 0);

	// VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_perpective_matrix = glm::perspective(glm::radians(60.f), (float)m_width/(float)m_height, 0.1f, 10.f);

	m_shader_cache["plane_vert"] = m_shaders.create_shader(GL_VERTEX_SHADER, "data/plane.vs");
	m_shader_cache["plane_frag"] = m_shaders.create_shader(GL_FRAGMENT_SHADER, "data/plane.fs");
	std::vector<std::shared_ptr<Shader>> plane_shader = {m_shader_cache["plane_vert"], m_shader_cache["plane_frag"]};
	m_programs["plane"] = m_shaders.create_program(plane_shader);
	
	m_shader_cache["suzanne_vert"] = m_shaders.create_shader(GL_VERTEX_SHADER, "data/suzanne.vs");
	m_shader_cache["suzanne_frag"] = m_shaders.create_shader(GL_FRAGMENT_SHADER, "data/suzanne.fs");
	std::vector<std::shared_ptr<Shader>> suzanne_shader = {m_shader_cache["suzanne_vert"], m_shader_cache["suzanne_frag"]};
	m_programs["suzanne"] = m_shaders.create_program(suzanne_shader);
}

void WindowHandler::rendering_loop()
{
	IndicedMesh plane(std::unique_ptr<std::vector<float>>(new std::vector<float>(plane_vert, plane_vert+sizeof(plane_vert)/sizeof(float))),
	                  std::unique_ptr<std::vector<GLuint>>(new std::vector<GLuint>(plane_ind, plane_ind+sizeof(plane_ind)+sizeof(GLuint))),
	                  GL_TRIANGLES, 3);
	plane.upload_to_gpu();
	
	// dirty - Mixup with reference and std::unique_ptr.
	std::unique_ptr<std::vector<float>> svert(new std::vector<float>());
	std::unique_ptr<std::vector<float>> snorm(new std::vector<float>());
	std::unique_ptr<std::vector<GLuint>> sind(new std::vector<GLuint>());
	
	load_obj("data/text.obj", svert.get(), sind.get(), snorm.get());
	IndicedMesh suzanne(std::move(svert), std::move(sind), GL_POINTS, 3);
	suzanne.upload_to_gpu();

	double mouse_x, mouse_y;
	glfwGetCursorPos(m_window, &mouse_x, &mouse_y);
	double mouse_dx = 0.0;
	double mouse_dy = 0.0;
	while (!glfwWindowShouldClose(m_window))
	{
		m_prev_t = glfwGetTime();

		// Clean Window.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(1.f, 1.f, 1.f, 0.f);

		if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
			m_camera->process_key(GLFW_KEY_W, m_frame_dt);

		if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
			m_camera->process_key(GLFW_KEY_S, m_frame_dt);

		if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
			m_camera->process_key(GLFW_KEY_A, m_frame_dt);

		if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
			m_camera->process_key(GLFW_KEY_D, m_frame_dt);

		if(glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
			m_camera->process_key(GLFW_KEY_SPACE, m_frame_dt);

		if(glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			m_camera->process_key(GLFW_KEY_LEFT_SHIFT, m_frame_dt);

		mouse_dx = mouse_x;
		mouse_dy = mouse_y;
		glfwGetCursorPos(m_window, &mouse_x, &mouse_y);
		mouse_dx -= mouse_x;
		mouse_dy -= mouse_y;

		if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			m_camera->process_mouse((float)mouse_dx, (float)mouse_dy, m_frame_dt);

		// Render stuff here.
		glUseProgram(m_programs["plane"]->addr);
		glUniformMatrix4fv(m_programs["plane"]->uniforms_location["camera"], 1, GL_FALSE, glm::value_ptr(m_perpective_matrix));
		glUniformMatrix4fv(m_programs["plane"]->uniforms_location["world"], 1, GL_FALSE, glm::value_ptr(m_camera->view()));
		plane.draw();
		
		glUseProgram(m_programs["suzanne"]->addr);
		glUniform1f(m_programs["suzanne"]->uniforms_location["time"], glfwGetTime());
		glUniformMatrix4fv(m_programs["suzanne"]->uniforms_location["camera"], 1, GL_FALSE, glm::value_ptr(m_perpective_matrix));
		glUniformMatrix4fv(m_programs["suzanne"]->uniforms_location["world"], 1, GL_FALSE, glm::value_ptr(m_camera->view()));
		glUniform4f(m_programs["suzanne"]->uniforms_location["eye"], m_camera->eye().x, m_camera->eye().y, m_camera->eye().z, 1.0);
		suzanne.draw();

		//m_particles->draw(m_perpective_matrix, m_camera->view(), glfwGetTime(), m_camera->eye());

		glUseProgram(0);

		// End of the loop stuff.
		m_particles->update_particules(m_frame_dt, 0.5);

		std::cout << std::fixed;
		std::cout.precision(0);
		std::cout << "\rfps: " << 1.0/m_frame_dt << " | Point drawed: " << m_max_part << " | colour: " << (m_particles->is_color_random() ?  "Random" : "Fixed") << "           ";

		// Swapping buffers & polling events.
		glfwSwapBuffers(m_window);
		glfwPollEvents();

		m_frame_dt = glfwGetTime()-m_prev_t;
	}
}

void WindowHandler::resize_callback(GLFWwindow* window, int width, int height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, width, height);
	m_perpective_matrix = glm::perspective(glm::radians(60.f), (float)m_width/(float)m_height, 0.1f, 10.f);
}

void WindowHandler::error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error (code: " << error << "): " << description << std::endl;
}

void WindowHandler::keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	//m_camera->process_key(key, m_frame_dt);

	if(key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		m_vsync = !m_vsync;
		glfwSwapInterval(m_vsync ? 1 : 0);
	}

	if(key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		m_particles->set_random_colour(!m_particles->is_color_random());
	}
}

void WindowHandler::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
}
