#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

GLuint create_shader(GLenum type, const std::string& shader_src);
GLuint create_program(const std::vector<GLuint> &shaders);
