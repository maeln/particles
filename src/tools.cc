#include <cstdio>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>

#include <sys/stat.h>
#include <sys/types.h>
#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#define stat _stat
#endif

#include "tools.hh"

std::string read_file(std::string path)
{
	std::ifstream f(path);
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

void load_obj(std::string filename, std::vector<float>* vertex, std::vector<GLuint>* indices, std::vector<float>* normals)
{
	std::ifstream file(filename.c_str(), std::ios::in);
	std::string line;

	float vert = 0;
	GLuint ind[3] = { 0, 0, 0 };
	GLuint nor[3] = { 0, 0, 0 };
	while (getline(file, line)) {
		if (line.substr(0, 2) == "v ") {
			std::istringstream str(line.substr(2));
			for (int n = 0; n < 3; ++n) {
				str >> vert;
				vertex->push_back(vert);
			}
		} else if (line.substr(0, 2) == "f ") {
			sscanf(line.substr(2).c_str(), "%d//%d %d//%d %d//%d\n", &ind[0], &nor[0], &ind[1], &nor[1], &ind[2], &nor[2]);
			indices->push_back(ind[0] - 1);
			indices->push_back(ind[1] - 1);
			indices->push_back(ind[2] - 1);
		} else if (line.substr(0, 3) == "vn ") {
			std::istringstream str(line.substr(3));
			for (int n = 0; n < 3; ++n) {
				str >> vert;
				normals->push_back(vert);
			}
		}
	}
}

time_t get_last_modified_time(std::string path)
{
	struct stat file_stat;
	if (stat(path.c_str(), &file_stat) == 0) {
		return file_stat.st_mtime;
	}

	std::cerr << "[ERR] Could not load stat for file: " << path << std::endl;
	return 0;
}
