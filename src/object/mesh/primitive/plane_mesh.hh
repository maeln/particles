#pragma once

#include "src/object/mesh/indiced_mesh.hh"

const float plane_vert[] = { -1.000000, -0.000000, 1.000000, 1.000000, -0.000000, 1.000000,
	-1.000000, -0.000000, -1.000000, 1.000000, -0.000000, -1.000000 };

const GLuint plane_ind[] = { 1, 2, 0, 1, 3, 2 };

class PlaneMesh : public IndicedMesh {
public:
	PlaneMesh()
		: IndicedMesh(
			  std::unique_ptr<std::vector<float>>(new std::vector<float>(plane_vert, plane_vert + sizeof(plane_vert) / sizeof(float))),
			  std::unique_ptr<std::vector<GLuint>>(new std::vector<GLuint>(plane_ind, plane_ind + sizeof(plane_ind) + sizeof(GLuint))),
			  GL_TRIANGLES, 3)
	{
	}

	~PlaneMesh() {}
};