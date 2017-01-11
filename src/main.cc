#include <iostream>
#include "window_handler.h"

/*
void program_plane(GLuint* program)
{
	std::vector<GLuint> shaders;
	shaders.push_back(create_shader(GL_VERTEX_SHADER, read_file("data/plane.vs"), "plane.vs"));
	shaders.push_back(create_shader(GL_FRAGMENT_SHADER, read_file("data/plane.fs"), "plane.fs"));
	*program = create_program(shaders);
	std::for_each(shaders.begin(), shaders.end(), glDeleteShader);
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
*/

int main(void)
{	
    WindowHandler& win = WindowHandler::instance();
    win.setup();
    win.rendering_loop();
    
    std::cout << std::endl;
    return 0;
}
