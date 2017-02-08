#include "basic_mesh.hh"

BasicMesh::BasicMesh(std::unique_ptr<std::vector<float>> vertex, GLenum render_mode, GLuint nb_vert_per_draw)
{
	m_vertex = std::move(vertex);
	m_mode = render_mode;
	m_vpd = nb_vert_per_draw;
}

BasicMesh::~BasicMesh()
{
	remove_from_gpu();
}

void BasicMesh::change_render_mode(GLenum render_mode, GLuint nb_vert_per_draw)
{
	m_mode = render_mode;
	m_vpd = nb_vert_per_draw;
}

void BasicMesh::upload_to_gpu()
{
	glGenBuffers(1, &m_v_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_v_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_vertex->size(), m_vertex->data(), GL_STATIC_DRAW);
}

void BasicMesh::remove_from_gpu()
{
	glDeleteBuffers(1, &m_v_vbo);
}

void BasicMesh::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_v_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements(GL_TRIANGLES, m_vertex->size()/m_vpd, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
}

