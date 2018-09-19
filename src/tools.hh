#pragma once

#include <GL/glew.h>
#include <memory>
#include <string>
#include <vector>

std::string read_file(std::string path);
void load_obj(std::string filename, std::vector<float>* vertex, std::vector<GLuint>* indices, std::vector<float>* normals);
time_t get_last_modified_time(std::string path);