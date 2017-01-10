#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

GLuint create_shader(GLenum type, const std::string& shader_src, const std::string& name);
GLuint create_program(const std::vector<GLuint> &shaders);
