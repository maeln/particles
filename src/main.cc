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
#include <cmath>

#include "tools.h"
#include "shader.h"
#include "data.h"

#define BENCHMARK 230000
#define MAX_POINT 230000
#define TTL 2.5

void init_program(GLuint* program)
{
	std::vector<GLuint> shaders;
	//shaders.push_back(create_shader(GL_GEOMETRY_SHADER, geometry_shader));
	shaders.push_back(create_shader(GL_VERTEX_SHADER, read_file("data/particle.vs")));
	shaders.push_back(create_shader(GL_FRAGMENT_SHADER, read_file("data/particle.fs")));
	*program = create_program(shaders);
	std::for_each(shaders.begin(), shaders.end(), glDeleteShader);
}

glm::vec3 lerp()
{
	float lp = (sin(glfwGetTime()/30.f)*sin(glfwGetTime()/30.f))*36.f;
	int lower_bound = floor(lp);
	int high_bound = ceil(lp);
	glm::vec3 pt1 = ece[lower_bound] * (float)(lp-floor(lp));
	glm::vec3 pt2 = ece[high_bound] * (float)(ceil(lp)-lp);
	glm::vec3 res = (pt1+pt2);
	return res/2.f;
}

std::default_random_engine gen;
std::uniform_real_distribution<double> distrib(-1.0, 1.0);
int eceit = 0;
void create_new_point(Point* p) 
{
	// Init random
	
	eceit++;
	if(eceit > 36)
		eceit = 0;
	
    p->pos = glm::vec4(ece[eceit]/2.f, 1.0);
    
    //p->pos = glm::vec4(lerp(), 1.0);
    //if(distrib(gen) > 0.0)
	//	p->dir = glm::vec4(sin(glfwGetTime()*2.f), cos(glfwGetTime()*2.f), distrib(gen), 0.0);
	//else
	//	p->dir = glm::vec4(-sin(glfwGetTime()*2.f), -cos(glfwGetTime()*2.f), distrib(gen), 0.0);
    
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
        glClearColor(1.f, 1.f, 1.f, 0.f);
        
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
