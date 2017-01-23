#include "indiced_mesh.hh"

IndicedMesh::IndicedMesh(std::unique_ptr<std::vector<float>> vertex, std::unique_ptr<std::vector<GLuint>> indices)
{
	m_vertex = std::move(vertex);
	m_indices = std::move(indices);
	m_in_gpu = false;
}

IndicedMesh::~IndicedMesh()
{
	remove_from_gpu();
}

void IndicedMesh::upload_to_gpu()
{
	glGenBuffers(1, &m_v_vbo);
	glGenBuffers(1, &m_i_vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_v_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_vertex->size(), m_vertex->data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_i_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_indices->size(), m_indices->data(), GL_STATIC_DRAW);
}

void IndicedMesh::remove_from_gpu()
{
	glDeleteBuffers(1, &m_v_vbo);
	glDeleteBuffers(1, &m_i_vbo);
}
	
void IndicedMesh::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_v_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_i_vbo);
	glDrawElements(GL_TRIANGLES, m_indices->size(), GL_UNSIGNED_INT, 0);
}
