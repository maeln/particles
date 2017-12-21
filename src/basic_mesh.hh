#pragma once

#include <GL/glew.h>
#include <vector>
#include <memory>

#include "shader_handler.hh"
#include "mesh.hh"

/**
 * Represent a Mesh that use non-indiced drawing.
 * The Mesh will just have a vertex buffer.
 */
class BasicMesh : public Mesh
{
public:
	BasicMesh(std::unique_ptr<std::vector<float>> vertex, std::shared_ptr<Program> program, GLenum render_mode, GLuint nb_vert_per_draw);
	~BasicMesh();

	void draw(GLuint camera_buffer);
	void upload_to_gpu();
	void remove_from_gpu();
	void change_render_mode(GLenum render_mode, GLuint nb_vert_per_draw);

private:
	std::unique_ptr<std::vector<float>> m_vertex;
	std::shared_ptr<Program> m_program;
	GLuint m_camera_loc;
	GLuint m_camera_bind;

	GLuint m_v_vbo;
	GLuint m_vao;
	GLenum m_mode;
	GLuint m_vpd;
};
