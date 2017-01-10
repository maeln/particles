#include "shader.h"
#include <iostream>

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
