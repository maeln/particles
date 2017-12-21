#include "basic_mesh.hh"

BasicMesh::BasicMesh(std::unique_ptr<std::vector<float>> vertex, std::shared_ptr<Program> program, GLenum render_mode, GLuint nb_vert_per_draw)
{
	m_vertex = std::move(vertex);
	m_mode = render_mode;
	m_vpd = nb_vert_per_draw;
	
	m_program = program;
	m_camera_loc = glGetUniformBlockIndex(m_program->addr, "camera");
	m_camera_bind = 0;
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_v_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}

void BasicMesh::remove_from_gpu()
{
	glDeleteBuffers(1, &m_v_vbo);
}

void BasicMesh::draw(GLuint camera_buffer)
{
	// TODO: Instead of doing the binding for the UBO in each draw and having to
	// specify the program in the mesh, we should create a Pass class which handle
	// the rendering side of things. Mesh and Particle class should be only about
	// the data storage and update.
	glBindBufferBase(GL_UNIFORM_BUFFER, m_camera_bind, camera_buffer);
	glUniformBlockBinding(m_program->addr, m_camera_loc, m_camera_bind);

	glBindVertexArray(m_vao);
	glUseProgram(m_program->addr);
	glDrawElements(GL_TRIANGLES, m_vertex->size()/m_vpd, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

