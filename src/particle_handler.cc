#include "particle_handler.hh"

#include <cmath>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>
#include <iostream>

ParticleHandler::ParticleHandler(GLuint nb_particule, float ttl_particule, float delta_ttl, glm::vec3 start_point, bool random_colour,
                                 glm::vec3 base_colour) {
    m_uniform = std::uniform_real_distribution<double>(-1.0, 1.0);
    m_max_part = nb_particule;
    m_ttl = ttl_particule;
    m_dttl = delta_ttl;
    m_pstart = start_point;

    m_rand_colour = random_colour;
    m_base_colour = base_colour;

    // Since the std140 memory layout will round to 16bytes anyway, we might as well use vec4 instead of vec3.
    m_part_pos.resize(m_max_part * 4);
    m_part_vel.resize(m_max_part * 4);
    m_part_ttl.resize(m_max_part, m_ttl);

    // Position
    for (GLuint n = 0; n < m_max_part; ++n) {
	m_part_pos[n * 4 + 0] = m_pstart.x;
	m_part_pos[n * 4 + 1] = m_pstart.y;
	m_part_pos[n * 4 + 2] = m_pstart.z;
	m_part_pos[n * 4 + 3] = 1.f;
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

    // shaders.
    std::shared_ptr<Shader> geom_shader = m_shaders.create_shader(GL_GEOMETRY_SHADER, "data/particle.gs");
    std::shared_ptr<Shader> vertex_shader = m_shaders.create_shader(GL_VERTEX_SHADER, "data/particle.vs");
    std::shared_ptr<Shader> pixel_shader = m_shaders.create_shader(GL_FRAGMENT_SHADER, "data/particle.fs");
    std::vector<std::shared_ptr<Shader>> ps = {geom_shader, vertex_shader, pixel_shader};
    m_program = m_shaders.create_program(ps);

    // Compute shader.
    std::shared_ptr<Shader> compute_shader = m_shaders.create_shader(GL_COMPUTE_SHADER, "data/particle.cs");
    std::vector<std::shared_ptr<Shader>> cs = {compute_shader};
    m_compute = m_shaders.create_program(cs);

    // For debug.
    // m_data = (float*)malloc(sizeof(float) * m_max_part * 3);

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

void ParticleHandler::update_particules(float dt, float speed_factor) {

    glUseProgram(m_compute->addr);
    glUniform1f(m_compute->uniforms_location["dt"], dt);
    glUniform1f(m_compute->uniforms_location["speed"], speed_factor);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_vbo_pos);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_vbo_vel);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_vbo_ttl);
    glDispatchCompute(m_max_part / 128, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    glUseProgram(0);

    /*
    // debug stuff
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
    m_data = (float*) glMapBufferRange(GL_ARRAY_BUFFER, 0, m_max_part * sizeof(float) * 3, GL_MAP_READ_BIT);
    std::setprecision(5);
    std::cout.precision(5);
    std::cout << "[--------------------------------------------" << std::endl;
    for (GLuint i = 0; i < m_max_part; ++i) {
            std::cout << " (" << m_data[i*3] << ", " << m_data[i*3+1] << ", " << m_data[i*3+2] << "; " << i << ")";
            if (i % 4 == 0)
                    std::cout << std::endl;
    }
    std::cout << "--------------------------------------------]" << std::endl;
    glUnmapBuffer(GL_ARRAY_BUFFER);
    */
}

void ParticleHandler::draw(glm::mat4 camera, glm::mat4 world, double time, glm::vec3 eye) {
    // Deactivate depth mask for higher performance.
    // Require back to front ordering.
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_program->addr);
    glUniform1f(m_program->uniforms_location["time"], time);
    glUniformMatrix4fv(m_program->uniforms_location["camera"], 1, GL_FALSE, glm::value_ptr(camera));
    glUniformMatrix4fv(m_program->uniforms_location["world"], 1, GL_FALSE, glm::value_ptr(world));
    glUniform4f(m_program->uniforms_location["eye"], eye.x, eye.y, eye.z, 1.0);

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
}

void ParticleHandler::set_colour(glm::vec3 col) { m_base_colour = col; }

void ParticleHandler::set_random_colour(bool r) { m_rand_colour = r; }

bool ParticleHandler::is_color_random() { return m_rand_colour; }
