#include "basic_mesh.hh"

BasicMesh::BasicMesh(std::unique_ptr<std::vector<float>> vertex, std::unique_ptr<std::vector<float>> tex_coord, GLenum render_mode,
	GLuint nb_vert_per_draw)
{
	m_vertex = std::move(vertex);
	m_tex_coord = std::move(tex_coord);
	m_mode = render_mode;
	m_vpd = nb_vert_per_draw;
}

BasicMesh::~BasicMesh() { remove_from_gpu(); }

void BasicMesh::change_render_mode(GLenum render_mode, GLuint nb_vert_per_draw)
{
	m_mode = render_mode;
	m_vpd = nb_vert_per_draw;
}

void BasicMesh::upload_to_gpu()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_v_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_v_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertex->size(), m_vertex->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_t_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_t_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_tex_coord->size(), m_tex_coord->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_v_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_t_vbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void BasicMesh::remove_from_gpu() { glDeleteBuffers(1, &m_v_vbo); }

void BasicMesh::draw()
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_vertex->size() / m_vpd);
	glBindVertexArray(0);
}
