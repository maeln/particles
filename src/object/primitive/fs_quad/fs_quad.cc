#include "fs_quad.hh"

#include <glm/gtc/type_ptr.hpp>

FSQuad::FSQuad(std::string fs)
	: SceneNode("fs_quad")
{
	auto program = m_shaderdb.load_program({ "data/shaders/post/post.vs", fs });
	if (program) {
		m_program = *program;
	} else {
		m_program = 0;
	}

	m_mesh.upload_to_gpu();
}

void FSQuad::draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 model)
{
	auto program = m_shaderdb.get_program(m_program);
	if (program) {
		glUseProgram(program->addr);
		glUniform1f(program->uniforms_location["time"], ctx->t_time);
		glUniform1f(program->uniforms_location["dt"], ctx->f_time);
		glUniform2f(program->uniforms_location["resolution"], ctx->v_width, ctx->v_height);
	}

	m_mesh.draw();
	glUseProgram(0);

	SceneNode::draw(ctx, model * transformation());
};