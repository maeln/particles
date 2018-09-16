#include "fs_quad.hh"

#include <glm/gtc/type_ptr.hpp>

FSQuad::FSQuad(std::string fs) {
    m_program = m_shaderdb.load_program({"data/shaders/post/post.vs", fs});
    m_mesh.upload_to_gpu();
}

void FSQuad::draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 model) {
    Program program = m_shaderdb.get_program(m_program);
    glUseProgram(program.addr);
    glUniform1f(program.uniforms_location["time"], ctx->t_time);
    glUniform1f(program.uniforms_location["dt"], ctx->f_time);
    glUniform2f(program.uniforms_location["resolution"], ctx->v_width, ctx->v_height);
    m_mesh.draw();
};