#include "shader_handler.hh"

#include <iostream>
#include <sstream>
#include "tools.hh"

std::shared_ptr<Shader> ShaderHandler::create_shader(GLenum type, const std::string& shader_src, const std::string& name)
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
	
	// If the compilation worked, we find all the uniform in the shader.
	std::shared_ptr<Shader> shader_obj(new Shader(shader));
	
	std::istringstream src(shader_src);
	std::string line;
	while(getline(src, line))
	{
		if(line.substr(0, 8) == "uniform ") 
		{
			std::istringstream str(line.substr(7));
			std::string type; std::string name;
			str >> type;
			str >> name;
			name.erase(name.find(";"), 1);
			shader_obj->uniforms.push_back(name);
		}
	}
	
	return shader_obj;
}

std::shared_ptr<Shader> ShaderHandler::create_shader(GLenum type, const std::string& filepath)
{
	std::string shader_src = read_file(filepath);
	return create_shader(type, shader_src, filepath);
}

std::shared_ptr<Program> ShaderHandler::create_program(std::vector<std::shared_ptr<Shader>>& shaders)
{
	GLuint program = glCreateProgram();
	
	for(size_t n=0; n<shaders.size(); ++n)
		glAttachShader(program, shaders[n]->addr);
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
		glDetachShader(program, shaders[n]->addr);
	
	// Manage uniform location.
	std::shared_ptr<Program> program_obj(new Program(program));
	for(GLuint n=0; n<shaders.size(); ++n)
	{
		for(GLuint u=0; u<shaders[n]->uniforms.size(); ++u)
		{
			if(program_obj->uniforms_location.find(shaders[n]->uniforms[u]) == program_obj->uniforms_location.end())
				program_obj->uniforms_location[shaders[n]->uniforms[u]] = glGetUniformLocation(program, shaders[n]->uniforms[u].c_str());
		}
	}
	
	return program_obj;
}
