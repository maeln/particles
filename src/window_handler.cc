#include "window_handler.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <exception>

#include "tools.h"
#include "data.h"

GLuint create_shader(GLenum type, const std::string& shader_src, const std::string& name)
{
	GLuint shader = glCreateShader(type);
	const char* shader_data = shader_src.c_str();
	glShaderSource(shader, 1, &shader_data, NULL);
	glCompileShader(shader);
	
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		GLint log_len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
		GLchar* log = new GLchar[log_len+1];
		glGetShaderInfoLog(shader, log_len, NULL, log);
		
		std::cerr << "Shader " << name << " compilation error: " << log << std::endl;
		delete[] log;
	}
	
	return shader;
}

GLuint create_program(const std::vector<GLuint> &shaders)
{
	GLuint program = glCreateProgram();
	
	for(size_t n=0; n<shaders.size(); ++n)
		glAttachShader(program, shaders[n]);
	glLinkProgram(program);
	
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		GLint log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		GLchar* log = new GLchar[log_length+1];
		glGetProgramInfoLog(program, log_length, NULL, log);
		std::cerr << "Program link failure: " << log << std::endl;
		delete[] log;
	}
	
	for(size_t n=0; n<shaders.size(); ++n)
		glDetachShader(program, shaders[n]);
	
	return program;
}

void program_plane(GLuint* program, GLint* loc_pers, GLint* loc_cam)
{
	std::vector<GLuint> shaders;
	shaders.push_back(create_shader(GL_VERTEX_SHADER, read_file("data/plane.vs"), "plane.vs"));
	shaders.push_back(create_shader(GL_FRAGMENT_SHADER, read_file("data/plane.fs"), "plane.fs"));
	*program = create_program(shaders);
	std::for_each(shaders.begin(), shaders.end(), glDeleteShader);
	
	*loc_pers = glGetUniformLocation(*program, "camera");
	*loc_cam = glGetUniformLocation(*program, "world");
}

void vbo_plane(GLuint* vbo_data, GLuint* vbo_ind)
{
	glGenBuffers(1, vbo_data);
	glGenBuffers(1, vbo_ind);
	
	glBindBuffer(GL_ARRAY_BUFFER, *vbo_data);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*plane_vert.size(), plane_vert.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *vbo_ind);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*plane_ind.size(), plane_ind.data(), GL_STATIC_DRAW); 
}

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
    
    m_mouse_dx = 0.d;
	m_mouse_dy = 0.d;
	m_frame_dt = 0.d;
	m_prev_t = 0.d;
	
	m_max_part = 1000;
	m_particles = new ParticleHandler(m_max_part, 2.5, 1.0);
	m_vsync = true;
	
	m_camera = new Camera(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f), 0.5, 1.0);
}

WindowHandler::~WindowHandler()
{
	delete m_particles;
	delete m_camera;
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
	
	m_shader_cache["part_vert"] = m_shaders.create_shader(GL_VERTEX_SHADER, "data/particle.vs");
	m_shader_cache["part_frag"] = m_shaders.create_shader(GL_FRAGMENT_SHADER, "data/particle.fs");
	std::vector<Shader*> part_shader = {m_shader_cache["part_vert"], m_shader_cache["part_frag"]};
	m_programs["particules"] = m_shaders.create_program(part_shader);
	
	m_shader_cache["plane_vert"] = m_shaders.create_shader(GL_VERTEX_SHADER, "data/plane.vs");
	m_shader_cache["plane_frag"] = m_shaders.create_shader(GL_FRAGMENT_SHADER, "data/plane.fs");
	std::vector<Shader*> plane_shader = {m_shader_cache["plane_vert"], m_shader_cache["plane_frag"]};
	m_programs["plane"] = m_shaders.create_program(plane_shader);
	
	program_plane(&mpp, &pers, &cam);
	vbo_plane(&mpd, &mpi);
}

void WindowHandler::rendering_loop()
{
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
		
		// Render stuff here.
		glm::mat4 camera_matrix = glm::lookAt(m_camera->eye(), m_camera->target(), m_camera->up());
		
		glUseProgram(m_programs["particules"]->addr);
        glUniform1f(m_programs["particules"]->uniforms_location["time"], glfwGetTime());
        glUniformMatrix4fv(m_programs["particules"]->uniforms_location["camera"], 1, GL_FALSE, glm::value_ptr(m_perpective_matrix));
        glUniformMatrix4fv(m_programs["particules"]->uniforms_location["world"], 1, GL_FALSE, glm::value_ptr(camera_matrix));
        glUniform4f(m_programs["particules"]->uniforms_location["eye"], m_camera->eye().x, m_camera->eye().y, m_camera->eye().z, 1.0);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_particles->get_vbo());
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_POINTS, 0, m_max_part);
        
        glUseProgram(mpp);
        glUniformMatrix4fv(pers, 1, GL_FALSE, glm::value_ptr(m_perpective_matrix));
        glUniformMatrix4fv(cam, 1, GL_FALSE, glm::value_ptr(camera_matrix));
		glBindBuffer(GL_ARRAY_BUFFER, mpd);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mpi);
		glDrawElements(GL_TRIANGLES, plane_ind.size(), GL_UNSIGNED_INT, 0);
        
        glDisableVertexAttribArray(0);
        glUseProgram(0);
        
        // End of the loop stuff.
        m_particles->update_particules(m_frame_dt, 0.5);
		
        std::cout << std::fixed;
		std::cout.precision(0);
		std::cout << "\rfps: " << 1.f/m_frame_dt << " | Point drawed: " << m_max_part << "           ";
		
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
}

void WindowHandler::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
}
