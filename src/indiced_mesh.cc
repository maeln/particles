#include "indiced_mesh.hh"

IndicedMesh::IndicedMesh(std::unique_ptr<std::vector<float>> vertex, std::unique_ptr<std::vector<GLuint>> indices, GLenum render_mode,
                         GLuint nb_vert_per_draw) {
    m_vertex = std::move(vertex);
    m_indices = std::move(indices);
    m_mode = render_mode;
    m_vpd = nb_vert_per_draw;
}

IndicedMesh::~IndicedMesh() { remove_from_gpu(); }

void IndicedMesh::change_render_mode(GLenum render_mode, GLuint nb_vert_per_draw) {
    m_mode = render_mode;
    m_vpd = nb_vert_per_draw;
}

void IndicedMesh::upload_to_gpu() {
    glGenBuffers(1, &m_v_vbo);
    glGenBuffers(1, &m_i_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_v_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertex->size(), m_vertex->data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_i_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices->size(), m_indices->data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_v_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_i_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, m_vpd, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
}

void IndicedMesh::remove_from_gpu() {
    glDeleteBuffers(1, &m_v_vbo);
    glDeleteBuffers(1, &m_i_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void IndicedMesh::draw() {
    glBindVertexArray(m_vao);
    glDrawElements(m_mode, m_indices->size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
