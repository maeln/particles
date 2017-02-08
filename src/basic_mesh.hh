#pragma once

#include <glad/glad.h>
#include <vector>
#include <memory>

#include "mesh.hh"

/**
 * Represent a Mesh that use non-indiced drawing.
 * The Mesh will just have a vertex buffer.
 */
class BasicMesh : public Mesh
{
public:
	BasicMesh(std::unique_ptr<std::vector<float>> vertex, GLenum render_mode, GLuint nb_vert_per_draw);
	~BasicMesh();

	void draw();
	void upload_to_gpu();
	void remove_from_gpu();
	void change_render_mode(GLenum render_mode, GLuint nb_vert_per_draw);

private:
	std::unique_ptr<std::vector<float>> m_vertex;
	GLuint m_v_vbo;
	GLenum m_mode;
	GLuint m_vpd;
};
