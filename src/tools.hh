#pragma once

#include <string>
#include <memory>
#include <vector>
#include <GL/glew.h>

std::string read_file(std::string path);
void load_obj(std::string filename, std::vector<float>* vertex, std::vector<GLuint>* indices, std::vector<float>* normals);
