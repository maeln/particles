#include "particle_handler.hh"

#include <cmath>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>
#include <iostream>

#include "src/image_handler.hh"

ParticleHandler::ParticleHandler(GLuint nb_particule, float ttl_particule, float delta_ttl, glm::vec3 start_point, glm::vec3 base_colour)
    : SceneNode("particules") {
    m_compute_supported = glewIsSupported("GL_ARB_compute_shader");
    m_uniform = std::uniform_real_distribution<float>(-1.0, 1.0);
    m_max_part = nb_particule;
    m_ttl = ttl_particule;
    m_dttl = delta_ttl;
    m_pstart = start_point;

    m_base_colour = base_colour;

    // Since the std140 memory layout will round to 16bytes anyway, we might as well use vec4 instead of vec3.
    m_part_pos.resize(m_max_part * 4);
    m_part_vel.resize(m_max_part * 4);
    m_part_ttl.resize(m_max_part, m_ttl);

    // Position
    if (m_compute_supported) {
	for (GLuint n = 0; n < m_max_part; ++n) {
	    m_part_pos[n * 4 + 0] = m_pstart.x;
	    m_part_pos[n * 4 + 1] = m_pstart.y;
	    m_part_pos[n * 4 + 2] = m_pstart.z;
	    m_part_pos[n * 4 + 3] = 1.f;
	}
    } else {
	for (GLuint n = 0; n < m_max_part; ++n) {
	    m_part_pos[n * 4 + 0] = m_uniform(m_randgen);
	    m_part_pos[n * 4 + 1] = m_uniform(m_randgen);
	    m_part_pos[n * 4 + 2] = m_uniform(m_randgen);
	    m_part_pos[n * 4 + 3] = 1.f;
	}
    }

    // Velocity
    for (GLuint n = 0; n < m_max_part; ++n) {
	m_part_vel[n * 4 + 0] = m_uniform(m_randgen);
	m_part_vel[n * 4 + 1] = m_uniform(m_randgen);
	m_part_vel[n * 4 + 2] = m_uniform(m_randgen);
	m_part_vel[n * 4 + 3] = 1.f;
    }

    if (m_dttl != 0) {
	std::uniform_real_distribution<double> unif = std::uniform_real_distribution<double>(-m_dttl, m_dttl);
	for (GLuint n = 0; n < m_part_ttl.size(); ++n) {
	    m_part_ttl[n] = ttl_particule + unif(m_randgen);
	}
    }

    glGenBuffers(1, &m_vbo_pos);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_part_pos.size(), m_part_pos.data(), GL_DYNAMIC_COPY);

    glGenBuffers(1, &m_vbo_vel);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vel);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_part_vel.size(), m_part_vel.data(), GL_DYNAMIC_COPY);

    glGenBuffers(1, &m_vbo_ttl);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_ttl);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_part_ttl.size(), m_part_ttl.data(), GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Texture
    // float tex_coord[] = {0.0f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f};
    m_texture = ImageHandler::load_texture("data/images/particle.png");

    // shaders.
    auto vprog = m_shaderdb.load_program(
        {"data/shaders/particle/particle.gs", "data/shaders/particle/particle.vs", "data/shaders/particle/particle.fs"});
    auto cprog = m_shaderdb.load_program({"data/shaders/particle/particle.cs"});

    if (vprog) {
	m_visual_program = *vprog;
    } else {
	m_visual_program = 0;
    }

    if (cprog) {
	m_compute_program = *cprog;
    } else {
	m_compute_program = 0;
    }

    // VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vel);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_ttl);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);

    glBindVertexArray(0);
}

ParticleHandler::~ParticleHandler() {
    glDeleteBuffers(1, &m_vbo_pos);
    glDeleteBuffers(1, &m_vbo_vel);
    glDeleteBuffers(1, &m_vbo_ttl);
    glDeleteVertexArrays(1, &m_vao);
}

void ParticleHandler::update_particules(float time, float dt, float speed_factor) {
    if (!m_compute_supported) {
	return;
    }
    auto program = m_shaderdb.get_program(m_compute_program);
    if (program) {
	glUseProgram(program->addr);
	glUniform1f(program->uniforms_location["time"], time);
	glUniform1f(program->uniforms_location["dt"], dt);
	glUniform1f(program->uniforms_location["speed"], speed_factor);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_vbo_pos);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_vbo_vel);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_vbo_ttl);
	glDispatchCompute(m_max_part / 128, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glUseProgram(0);
    }
}

void ParticleHandler::draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 model) {
    glm::mat4 parts_model = model * transformation();

    // Update position
    update_particules(ctx->t_time, ctx->f_time);

    // Deactivate depth mask for higher performance.
    // Require back to front ordering.
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    auto program = m_shaderdb.get_program(m_visual_program);
    if (program) {
	glUseProgram(program->addr);
	glUniform1f(program->uniforms_location["time"], ctx->t_time);
	glUniform1f(program->uniforms_location["dt"], ctx->f_time);

	glUniform4f(program->uniforms_location["part_colour"], m_base_colour.r, m_base_colour.g, m_base_colour.b, 1.0);
	glUniform4f(program->uniforms_location["eye"], ctx->activeCamera->eye().x, ctx->activeCamera->eye().y, ctx->activeCamera->eye().z,
	            1.0);

	glUniformMatrix4fv(program->uniforms_location["view"], 1, GL_FALSE, glm::value_ptr(ctx->activeCamera->view()));
	glUniformMatrix4fv(program->uniforms_location["projection"], 1, GL_FALSE, glm::value_ptr(ctx->perspective));
	glUniformMatrix4fv(program->uniforms_location["model"], 1, GL_FALSE, glm::value_ptr(parts_model));
    }

    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, m_max_part);
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glUseProgram(0);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    SceneNode::draw(ctx, parts_model);
}

void ParticleHandler::set_colour(glm::vec3 col) { m_base_colour = col; }
glm::vec3 ParticleHandler::get_colour() { return m_base_colour; }
