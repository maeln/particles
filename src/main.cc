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
#include <random>
#include "data.h"

#define BENCHMARK 230000
#define MAX_POINT 1000
#define TTL 2.5

GLuint create_shader(GLenum type, const std::string& shader_src)
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
		
		std::cerr << "Shader compilation error: " << log << std::endl;
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

void init_program(GLuint* program)
{
	std::vector<GLuint> shaders;
	//shaders.push_back(create_shader(GL_GEOMETRY_SHADER, geometry_shader));
	shaders.push_back(create_shader(GL_VERTEX_SHADER, vertex_shader));
	shaders.push_back(create_shader(GL_FRAGMENT_SHADER, fragment_shader));
	*program = create_program(shaders);
	std::for_each(shaders.begin(), shaders.end(), glDeleteShader);
}

std::default_random_engine gen;
std::uniform_real_distribution<double> distrib(-1.0, 1.0);
void create_new_point(Point* p) 
{
	// Init random
    p->pos = glm::vec4(0.0, 0.0, 0.0, 1.0);
    p->dir = glm::vec4(distrib(gen), distrib(gen), distrib(gen), 0.0);
    p->ttl = TTL+(TTL*(distrib(gen)/2.0));
}

void update_point(Point* p, double dt)
{
	if((p->ttl - dt) <= 0)
		create_new_point(p);
	else
	{
		glm::vec4 speed(dt/2.0);
		p->pos += (p->dir*speed);
		p->ttl = p->ttl - dt;
	}
}

void vbo_point(std::vector<Point>& points, float* data, GLuint* vbo, bool update)
{
	for(size_t n=0; n<points.size(); ++n)
	{
		if(update)
		{
			data[n*4] = points[n].pos.x;
			data[n*4+1] = points[n].pos.y;
			data[n*4+2] = points[n].pos.z;
			data[n*4+3] = points[n].pos.w;
		}
		else
		{
			data[n*4] = 0;
			data[n*4+1] = 0;
			data[n*4+2] = 0;
			data[n*4+3] = 0;
		}
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	if(update)
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*4*points.size(), data);
	else
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*points.size(), data, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int main(void)
{	
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
	
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
	
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    
    // Init data
    GLuint vbo, vao, program;
    glGenBuffers(1, &vbo);
    init_program(&program);
    
    // VAO
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	// Face culling
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
    // Time data
    double prev = 0.0;
    double curr = 0.0;
    double frameTime = 0.0;
    
    // Init Points
    std::vector<Point> points;
    for(size_t n=0; n<MAX_POINT; ++n)
    {
		Point tmp = {glm::vec4(0), glm::vec4(0), 0};
		points.push_back(tmp);
	}
    float* data = new float[4*points.size()];
    for(size_t n=0; n<points.size(); ++n)
		update_point(&points[n], 0);
	vbo_point(points, data, &vbo, false);
	
	glfwSwapInterval(1);
	
	GLint time = glGetUniformLocation(program, "time");
	GLint camera_location = glGetUniformLocation(program, "camera");
	glm::mat4 camera_matrix = glm::perspective(glm::radians(90.f), 1.33f, 0.1f, 10.f);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
		curr = glfwGetTime();
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(program);
        glUniform1f(time, glfwGetTime());
        glUniformMatrix4fv(camera_location, 1, GL_FALSE, glm::value_ptr(camera_matrix));
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_POINTS, 0, points.size());
        glDisableVertexAttribArray(0);
        glUseProgram(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        
        for(size_t n=0; n<points.size(); ++n)
			update_point(&points[n], frameTime);
		vbo_point(points, data, &vbo, true);
		
		std::cout << std::fixed;
		std::cout.precision(8);
		std::cout << "\rfps: " << 1.d/frameTime << " | Point drawed :" << points.size()
			<< " | TTL1: " << points[0].ttl;
        
        prev = glfwGetTime();
        frameTime = prev-curr;
    }
	
	delete[] data;
	
    glfwTerminate();
    return 0;
}
