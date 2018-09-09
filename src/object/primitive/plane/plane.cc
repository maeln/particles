#include "plane.hh"

#include <glm/gtc/type_ptr.hpp>

Plane::Plane(std::shared_ptr<Program> program) {
    m_program = program;
    m_mesh.upload_to_gpu();
}

Plane::~Plane() { m_mesh.remove_from_gpu(); }

void Plane::draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 model) {
    glUseProgram(m_program->addr);
    glUniform1f(m_program->uniforms_location["time"], ctx->t_time);
    glUniform1f(m_program->uniforms_location["dt"], ctx->f_time);
    glUniformMatrix4fv(m_program->uniforms_location["view"], 1, GL_FALSE, glm::value_ptr(ctx->activeCamera->view()));
    glUniformMatrix4fv(m_program->uniforms_location["projection"], 1, GL_FALSE, glm::value_ptr(ctx->perspective));
    glUniformMatrix4fv(m_program->uniforms_location["model"], 1, GL_FALSE, glm::value_ptr(model));
    m_mesh.draw();
}