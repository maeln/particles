#pragma once

#include <GL/glew.h>
#include <memory>
#include <vector>

#include "mesh.hh"

/**
 * Represent a Mesh that use indiced drawing.
 * The Mesh will have a vertex buffer and an indice buffer.
 */
class IndicedMesh : public Mesh {
public:
	IndicedMesh(std::unique_ptr<std::vector<float>> vertex, std::unique_ptr<std::vector<GLuint>> indices, GLenum render_mode, GLuint nb_vert_per_draw);
	~IndicedMesh();

	void draw();
	void upload_to_gpu();
	void remove_from_gpu();
	void change_render_mode(GLenum render_mode, GLuint nb_vert_per_draw);

private:
	std::unique_ptr<std::vector<float>> m_vertex;
	std::unique_ptr<std::vector<GLuint>> m_indices;
	GLuint m_v_vbo;
	GLuint m_i_vbo;
	GLuint m_vao;
	GLenum m_mode;
	GLuint m_vpd;
};
