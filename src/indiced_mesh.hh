#pragma once

#include <glad/glad.h>
#include <vector>
#include <memory>

#include "mesh.hh"

/** 
 * Represent a Mesh that use indiced drawing.
 * The Mesh will have a vertex buffer and an indice buffer.
 */
class IndicedMesh : public Mesh
{
public:
	IndicedMesh(std::unique_ptr<std::vector<float>> vertex, std::unique_ptr<std::vector<GLuint>> indices);
	~IndicedMesh();
	
	void draw();
	void upload_to_gpu();
	void remove_from_gpu();
	
private:
	std::unique_ptr<std::vector<float>> m_vertex;
	std::unique_ptr<std::vector<GLuint>> m_indices;
	GLuint m_v_vbo;
	GLuint m_i_vbo;
	bool m_in_gpu;
};
