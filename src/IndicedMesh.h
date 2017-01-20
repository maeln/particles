#pragma once

#include <glad/glad.h>
#include <vector>
#include "mesh.h"

/** 
 * Represent a Mesh that use indiced drawing.
 * The Mesh will have a vertex buffer and an indice buffer.
 */
class IndicedMesh : Mesh
{
public:
	Mesh(std::vector<float>& vertex, std::vector<GLuint>& indices);
	~Mesh();
	
	void draw();
	
private:
	std::vector<float> m_vertex;
	std::vector<GLuint> m_indices;
	GLuint m_v_vbo;
	GLuint m_i_vbo;
};
