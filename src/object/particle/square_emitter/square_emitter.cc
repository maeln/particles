#include "square_emitter.hh"

#include "src/image_handler.hh"

#include <glm/gtc/type_ptr.hpp>
#include <vector>

SquareEmitter::SquareEmitter(glm::vec3 square, glm::vec3 density, glm::vec4 color)
	: SceneNode("square_emitter")
{
	m_square = square;
	m_density = density;
	m_color = color;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_point);
	glBindBuffer(GL_ARRAY_BUFFER, m_point);
	float d[8] = { 0.0, 0.0, 0.0, 1.0, color.r, color.g, color.b, color.a };
	glBufferData(GL_ARRAY_BUFFER, sizeof(d), d, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(4 * sizeof(float)));

	m_nbparts = static_cast<GLuint>(density.x * density.y * density.z);
	std::vector<float> offset = std::vector<float>(m_nbparts * 4, 0.0);
	glGenBuffers(1, &m_offset);
	glBindBuffer(GL_ARRAY_BUFFER, m_offset);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * offset.size(), offset.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glVertexAttribDivisor(2, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_texture = ImageHandler::load_texture("data/images/particle.png");

	m_material = 0;
	auto material = m_shadb.load_program({ "data/shaders/particle/square/square.vs", "data/shaders/particle/square/square.gs", "data/shaders/particle/square/square.fs" });
	if (material)
		m_material = *material;

	auto init = m_shadb.load_program({ "data/shaders/particle/square/init.cs" });
	if (init)
		m_initializer = *init;

	auto updater = m_shadb.load_program({ "data/shaders/particle/square/update.cs" });
	if (updater)
		m_updater = *updater;

	auto program = m_shaderdb.get_program(m_initializer);
	if (program) {
		glUseProgram(program->addr);
		glUniform3f(program->uniforms_location["square"], m_square.x, m_square.y, m_square.z);
		glUniform3f(program->uniforms_location["density"], m_density.x, m_density.y, m_density.z);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_offset);
		glDispatchCompute(m_nbparts / 128, 1, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		glUseProgram(0);
	}
}

SquareEmitter::~SquareEmitter()
{
}

void SquareEmitter::update_particles(std::shared_ptr<SceneContext> ctx)
{
}

void SquareEmitter::draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 model)
{
	glm::mat4 parts_model = model * transform();

	// Update position
	update_particles(ctx);

	// Deactivate depth mask for higher performance.
	// Require back to front ordering.
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	auto program = m_shaderdb.get_program(m_material);
	if (program) {
		glUseProgram(program->addr);
		glUniform1f(program->uniforms_location["time"], ctx->t_time);
		glUniform1f(program->uniforms_location["dt"], ctx->f_time);

		glUniform4f(program->uniforms_location["base"], m_color.r, m_color.g, m_color.b, m_color.a);
		glUniform4f(program->uniforms_location["eye"], ctx->activeCamera->eye().x, ctx->activeCamera->eye().y, ctx->activeCamera->eye().z,
			1.0);

		glUniformMatrix4fv(program->uniforms_location["view"], 1, GL_FALSE, glm::value_ptr(ctx->activeCamera->view()));
		glUniformMatrix4fv(program->uniforms_location["projection"], 1, GL_FALSE, glm::value_ptr(ctx->perspective));
		glUniformMatrix4fv(program->uniforms_location["model"], 1, GL_FALSE, glm::value_ptr(parts_model));
	}

	glBindVertexArray(m_vao);
	glDrawArraysInstanced(GL_POINTS, 0, 1, m_nbparts);
	glBindVertexArray(0);

	glUseProgram(0);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	SceneNode::draw(ctx, parts_model);
}
