#include "plane.hh"

#include <glm/gtc/type_ptr.hpp>

Plane::Plane() : SceneNode("plane") {
    auto program = m_shaderdb.load_program({"data/shaders/plane/plane.vs", "data/shaders/plane/plane.fs"});
    if (program) {
	m_program = *program;
    } else {
	m_program = 0;
    }

    m_mesh.upload_to_gpu();
}

Plane::~Plane() { m_mesh.remove_from_gpu(); }

void Plane::draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 model) {
    glm::mat4 m_model = model * transformation();

    auto program = m_shaderdb.get_program(m_program);
    if (program) {
	glUseProgram(program->addr);
	glUniform1f(program->uniforms_location["time"], ctx->t_time);
	glUniform1f(program->uniforms_location["dt"], ctx->f_time);
	glUniformMatrix4fv(program->uniforms_location["view"], 1, GL_FALSE, glm::value_ptr(ctx->activeCamera->view()));
	glUniformMatrix4fv(program->uniforms_location["projection"], 1, GL_FALSE, glm::value_ptr(ctx->perspective));
	glUniformMatrix4fv(program->uniforms_location["model"], 1, GL_FALSE, glm::value_ptr(m_model));
    }
    m_mesh.draw();
    glUseProgram(0);

    SceneNode::draw(ctx, m_model);
}