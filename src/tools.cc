#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <cstdio>

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
	GLuint ind[3] = {0, 0, 0};
	GLuint nor[3] = {0, 0, 0};
	while(getline(file, line))
	{
		if(line.substr(0,2) == "v ")
		{
			std::istringstream str(line.substr(2));
			for(int n=0; n<3; ++n)
			{
				str >> vert;
				vertex->push_back(vert);
			}
		}
		else if(line.substr(0,2) == "f ")
		{
			sscanf(line.substr(2).c_str(), "%d//%d %d//%d %d//%d\n", &ind[0], &nor[0], &ind[1], &nor[1], &ind[2], &nor[2]);
			indices->push_back(ind[0]-1);
			indices->push_back(ind[1]-1);
			indices->push_back(ind[2]-1);
		}
		else if(line.substr(0,3) == "vn ")
		{
			std::istringstream str(line.substr(3));
			for(int n=0; n<3; ++n)
			{
				str >> vert;
				normals->push_back(vert);
			}
		}
	}
	
	// Process the normals
	/*
	for(unsigned int i = 0; i < indices.size(); i += 3)
	{
		GLuint ia = indices[i];
		GLuint ib = indices[i+1];
		GLuint ic = indices[i+2];
		
		glm::vec3 va = glm::vec3(vertex[ia*3], vertex[ia*3+1], vertex[ia*3+2]);
		glm::vec3 vb = glm::vec3(vertex[ib*3], vertex[ib*3+1], vertex[ib*3+2]);
		glm::vec3 vc = glm::vec3(vertex[ic*3], vertex[ic*3+1], vertex[ic*3+2]);
		
		glm::vec3 tmp1 = vb - va;
		glm::vec3 tmp2 = vc - va;
		glm::vec3 normal = glm::normalize(glm::cross(tmp1, tmp2));
		normals[ia*3] = normals[ib*3] = normals[ic*3] = normal.x;
		normals[ia*3+1] = normals[ib*3+1] = normals[ic*3+1] = normal.y;
		normals[ia*3+2] = normals[ib*3+2] = normals[ic*3+2] = normal.z;
	}
	*/
}
