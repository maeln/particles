#include "particle_handler.h"

ParticleHandler::ParticleHandler(GLuint nb_particule, float ttl_particule, float delta_ttl)
{
	m_uniform = std::uniform_real_distribution<double>(-1.0, 1.0);
	m_max_part = nb_particule;
	m_ttl = ttl_particule;
	m_dttl = delta_ttl;
	
	m_part_pos.resize(m_max_part*3, 0.f);
	m_part_vel.resize(m_max_part*3);
	m_part_ttl.resize(m_max_part, m_ttl);
	
	for(GLuint n=0; n<m_part_vel.size(); ++n)
	{
		m_part_vel[n] = m_uniform(m_randgen);
	}
	
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
}

ParticleHandler::~ParticleHandler()
{
	glDeleteBuffers(1, &m_vbo);
}
	
void ParticleHandler::update_particules(float dt, float speed_factor)
{
	if(dt > 0)
	{
		for(GLuint n=0; n<m_part_vel.size(); ++n)
		{
			m_part_pos[n] += m_part_vel[n]*speed_factor;
		}
		
		for(GLuint n=0; n<m_part_ttl.size(); ++n)
		{
			m_part_ttl[n] -= dt;
			if(m_part_ttl[n] <= 0) // Particle is dead. Reset pos & vel.
			{
				m_part_pos[n*3] = 0.f;
				m_part_pos[n*3+1] = 0.f;
				m_part_pos[n*3+2] = 0.f;
				m_part_vel[n*3] = m_uniform(m_randgen);
				m_part_vel[n*3+1] = m_uniform(m_randgen);
				m_part_vel[n*3+2] = m_uniform(m_randgen);
				m_part_ttl[n] = m_ttl + m_uniform(m_randgen) * m_dttl;
			}
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*m_part_pos.size(), m_part_pos.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

GLuint ParticleHandler::get_vbo()
{
	return m_vbo;
}
