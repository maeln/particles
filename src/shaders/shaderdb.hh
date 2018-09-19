#pragma once

#include <GL/glew.h>
#include <exception>
#include <initializer_list>
#include <map>
#include <optional>
#include <string>
#include <vector>

class Shader {
  public:
    Shader(){};
    Shader(GLuint h, GLuint a, std::string p, time_t m) {
	handler = h;
	addr = a;
	path = p;
	last_modified = m;
    };

    GLuint handler;
    GLuint addr;
    std::vector<std::string> uniforms;
    std::string path;
    time_t last_modified;
};

class Program {
  public:
    Program(){};
    Program(GLuint h, GLuint a) {
	handler = h;
	addr = a;
    };

    GLuint handler;
    GLuint addr;

    std::vector<GLuint> shaders;
    std::map<std::string, GLuint> uniforms_location;
};

/** Shader Database
 * use to load, parse, compile and keep references of shaders and programs.
 */
class ShaderDB {
  public:
    static ShaderDB &instance() {
	static ShaderDB instance;
	return instance;
    }
    ~ShaderDB() {
	for (auto shader : m_shaders) {
	    glDeleteShader(shader.second.addr);
	}
	for (auto program : m_programs) {
	    glDeleteProgram(program.second.addr);
	}
    };

    std::optional<GLuint> load_program(const std::initializer_list<std::string> shaders);
    void check_reload();
    std::optional<Shader> get_shader(GLuint handler);
    std::optional<Program> get_program(GLuint handler);

  private:
    ShaderDB() {
	m_program_counter = 1;
	m_shader_counter = 1;
    };
    ShaderDB(const ShaderDB &);
    void operator=(const ShaderDB &);

    std::optional<Shader> load_shader(std::string path);
    GLenum shader_type(std::string path);

    bool reload_program(GLuint handler);

    GLuint m_program_counter;
    GLuint m_shader_counter;

    std::map<GLuint, Shader> m_shaders;
    std::map<GLuint, Program> m_programs;

    std::map<std::string, GLuint> m_shaders_path;
};
