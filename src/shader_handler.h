#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <map>

class Program
{
public:
	GLuint addr;
	std::map<std::string, GLint> uniforms_location;
	Program(GLuint p) {addr = p;};
	~Program() {glDeleteProgram(addr);};
};

class Shader
{
public:
	GLuint addr;
	std::vector<std::string> uniforms;
	Shader(GLuint s) {addr = s;};
	~Shader() {glDeleteShader(addr);};
};

class ShaderHandler
{
public:
	static ShaderHandler& instance()
	{
		static ShaderHandler instance;
		return instance;
	}
	
	Shader* create_shader(GLenum type, const std::string& shader_src, const std::string& name);
	Shader* create_shader(GLenum type, const std::string& filepath);
	Program* create_program(std::vector<Shader*>& shaders);
	
private:
	ShaderHandler() {};
	ShaderHandler(ShaderHandler const&);
	void operator=(ShaderHandler const&);
};
