#pragma once

#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include <GL/glew.h>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/shaders/shaderdb.hh"

class MaterialData {
public:
	MaterialData() {}
	MaterialData(std::shared_ptr<MaterialData> other)
	{
		floats.merge(other->floats);
		vec2.merge(other->vec2);
		vec3.merge(other->vec3);
		vec4.merge(other->vec4);
		mat4.merge(other->mat4);
	}

	~MaterialData() {}

	std::map<std::string, float> floats;
	std::map<std::string, glm::vec2> vec2;
	std::map<std::string, glm::vec3> vec3;
	std::map<std::string, glm::vec4> vec4;
	std::map<std::string, glm::mat4> mat4;
};

class Material {
public:
	Material()
	{
		m_program = 0;
	}

	Material(std::initializer_list<std::string> shaders)
	{
		std::optional<GLuint> program_handler = m_shdb.load_program(shaders);
		if (program_handler) {
			m_program = *program_handler;
		} else {
			std::cerr << "Could not create program with shaders: ";
			for (auto shader : shaders) {
				std::cerr << shader << "; ";
			}
			std::cerr << std::endl;

			m_program = 0;
		}
	}

	~Material() {}

	void set_program(GLuint handler) { m_program = handler; }
	GLuint get_program() { return m_program; }

	void use()
	{
		auto program = m_shdb.get_program(m_program);
		if (program) {
			glUseProgram(program->addr);
		}
	}

	void release()
	{
		glUseProgram(0);
	}

	void set_data(std::shared_ptr<MaterialData> material_data)
	{
		auto program = m_shdb.get_program(m_program);
		if (program) {
			for (auto data : material_data->floats) {
				glUniform1f(program->uniforms_location[data.first], data.second);
			}

			for (auto data : material_data->vec2) {
				glUniform2fv(program->uniforms_location[data.first], 1, glm::value_ptr(data.second));
			}

			for (auto data : material_data->vec3) {
				glUniform3fv(program->uniforms_location[data.first], 1, glm::value_ptr(data.second));
			}

			for (auto data : material_data->vec4) {
				glUniform4fv(program->uniforms_location[data.first], 1, glm::value_ptr(data.second));
			}

			for (auto data : material_data->mat4) {
				glUniformMatrix4fv(program->uniforms_location[data.first], 1, GL_FALSE, glm::value_ptr(data.second));
			}
		}
	}

private:
	GLuint m_program;
	ShaderDB& m_shdb = ShaderDB::instance();
};
