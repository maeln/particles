#pragma once

#include "src/object/mesh/indiced_mesh.hh"

const float cube_vert[] = {
	-1.000000,
	-1.000000,
	1.000000,
	-1.000000,
	1.000000,
	1.000000,
	-1.000000,
	-1.000000,
	-1.000000,
	-1.000000,
	1.000000,
	-1.000000,
	1.000000,
	-1.000000,
	1.000000,
	1.000000,
	1.000000,
	1.000000,
	1.000000,
	-1.000000,
	-1.000000,
	1.000000,
	1.000000,
	-1.000000,
};

const float cube_normals[] = {
	-1.0000,
	0.0000,
	0.0000,
	0.0000,
	0.0000,
	-1.0000,
	1.0000,
	0.0000,
	0.0000,
	0.0000,
	0.0000,
	1.0000,
	0.0000,
	-1.0000,
	0.0000,
	0.0000,
	1.0000,
	0.0000,
};

const GLuint cube_ind[] = {
	1,
	0,
	2,
	0,
	0,
	0,
	3,
	1,
	6,
	1,
	2,
	1,
	7,
	2,
	4,
	2,
	6,
	2,
	5,
	3,
	0,
	3,
	4,
	3,
	6,
	4,
	0,
	4,
	2,
	4,
	3,
	5,
	5,
	5,
	7,
	5,
	1,
	0,
	3,
	0,
	2,
	0,
	3,
	1,
	7,
	1,
	6,
	1,
	7,
	2,
	5,
	2,
	4,
	2,
	5,
	3,
	1,
	3,
	0,
	3,
	6,
	4,
	4,
	4,
	0,
	4,
	3,
	5,
	1,
	5,
	5,
	5,
};

class CubeMesh : public IndicedMesh {
public:
	CubeMesh()
		: IndicedMesh(
			  std::unique_ptr<std::vector<float>>(new std::vector<float>(cube_vert, cube_vert + sizeof(cube_vert) / sizeof(float))),
			  std::unique_ptr<std::vector<GLuint>>(new std::vector<GLuint>(cube_ind, cube_ind + sizeof(cube_ind) + sizeof(GLuint))),
			  GL_TRIANGLES, 3)
	{
	}

	~CubeMesh() {}
};