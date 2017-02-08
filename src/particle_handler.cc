#include "particle_handler.hh"

#include <glm/gtc/type_ptr.hpp>
#include <cmath>

ParticleHandler::ParticleHandler(GLuint nb_particule, float ttl_particule, float delta_ttl, glm::vec3 start_point, bool random_colour, glm::vec3 base_colour)
{
	m_uniform = std::uniform_real_distribution<double>(-1.0, 1.0);
	m_max_part = nb_particule;
	m_ttl = ttl_particule;
	m_dttl = delta_ttl;
	m_pstart = start_point;

	m_rand_colour = random_colour;
	m_base_colour = base_colour;

	m_part_pos.resize((m_max_part*3)*2);
	m_part_vel.resize(m_max_part*3);
	m_part_ttl.resize(m_max_part, m_ttl);

	// Position
	for(GLuint n=0; n<m_max_part; ++n)
	{
		m_part_pos[n*3] = m_pstart.x;
		m_part_pos[n*3+1] = m_pstart.y;
		m_part_pos[n*3+2] = m_pstart.z;
	}

	// Colour
	if(m_rand_colour)
	{
		for(GLuint n=0; n<(m_max_part*3); ++n)
		{
			m_part_pos[m_max_part*3+n] = std::abs(m_uniform(m_randgen));
		}
	}
	else
	{
		GLuint offset = m_max_part*3;
		for(GLuint n=0; n<m_max_part; ++n)
		{
			m_part_pos[offset+n*3] = m_base_colour.r;
			m_part_pos[offset+n*3+1] = m_base_colour.g;
			m_part_pos[offset+n*3+2] = m_base_colour.b;
		}
	}

	// Velocity
	for(GLuint n=0; n<m_part_vel.size(); ++n)
	{
		m_part_vel[n] = m_uniform(m_randgen);
	}

	// Time to live
	if(m_dttl > 0)
	{
		for(GLuint n=0; n<m_part_ttl.size(); ++n)
		{
			m_part_ttl[n] += m_uniform(m_randgen) * m_dttl;
		}
	}

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_part_pos.size(), m_part_pos.data(), GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// shaders.
	std::shared_ptr<Shader> vertex_shader = m_shaders.create_shader(GL_VERTEX_SHADER, "data/particle.vs");
	std::shared_ptr<Shader> pixel_shader = m_shaders.create_shader(GL_FRAGMENT_SHADER, "data/particle.fs");
	std::vector<std::shared_ptr<Shader>> part_shader = {vertex_shader, pixel_shader};
	m_program = m_shaders.create_program(part_shader);
}

ParticleHandler::~ParticleHandler()
{
	glDeleteBuffers(1, &m_vbo);
}

void ParticleHandler::update_particules(float dt, float speed_factor)
{
	for(GLuint n=0; n<m_part_vel.size(); ++n)
	{
		m_part_pos[n] += m_part_vel[n]*speed_factor*dt;
	}

	for(GLuint n=0; n<m_max_part; ++n)
	{
		m_part_ttl[n] -= dt;
		if(m_part_ttl[n] <= 0) // Particle is dead. Reset pos & vel.
		{
			m_part_pos[n*3] = m_pstart.x;
			m_part_pos[n*3+1] = m_pstart.y;
			m_part_pos[n*3+2] = m_pstart.z;

			m_part_vel[n*3] = m_uniform(m_randgen);
			m_part_vel[n*3+1] = m_uniform(m_randgen);
			m_part_vel[n*3+2] = m_uniform(m_randgen);

			m_part_ttl[n] = m_ttl + m_uniform(m_randgen) * m_dttl;

			GLuint offset = m_max_part*3;
			if(m_rand_colour)
			{
				m_part_pos[offset+n*3] = std::abs(m_uniform(m_randgen));
				m_part_pos[offset+n*3+1] = std::abs(m_uniform(m_randgen));
				m_part_pos[offset+n*3+2] = std::abs(m_uniform(m_randgen));
			}
			else
			{
				m_part_pos[offset+n*3] = m_base_colour.r;
				m_part_pos[offset+n*3+1] = m_base_colour.g;
				m_part_pos[offset+n*3+2] = m_base_colour.b;
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*m_part_pos.size(), m_part_pos.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleHandler::draw(glm::mat4 camera, glm::mat4 world, double time, glm::vec3 eye)
{
	glUseProgram(m_program->addr);
	glUniform1f(m_program->uniforms_location["time"], time);
	glUniformMatrix4fv(m_program->uniforms_location["camera"], 1, GL_FALSE, glm::value_ptr(camera));
	glUniformMatrix4fv(m_program->uniforms_location["world"], 1, GL_FALSE, glm::value_ptr(world));
	glUniform4f(m_program->uniforms_location["eye"], eye.x, eye.y, eye.z, 1.0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) (long) (m_max_part*3*sizeof(float)));
	glDrawArrays(GL_POINTS, 0, m_max_part);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);
}

void ParticleHandler::set_colour(glm::vec3 col)
{
	m_base_colour = col;
}

void ParticleHandler::set_random_colour(bool r)
{
	m_rand_colour = r;
}

bool ParticleHandler::is_color_random()
{
	return m_rand_colour;
}
