#include "particle_handler.h"

ParticleHandler::ParticleHandler(GLuint nb_particule, float ttl_particule, float delta_ttl)
{
	m_max_part = nb_particule;
	m_part_ttl = ttl_particule;
	m_dttl = delta_ttl;
	
	m_part_pos.resize(m_max_part*3, 0.f);
	m_part_vel.resize(m_max_part*3);
	m_part_ttl.resize(m_max_part, m_part_ttl);
	
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
}
	
void ParticleHandler::update_particules()
{
	// TODO: MINUS TTL
	// TODO: VEL SPEED
	for(GLuint n=0; n<m_part_vel.size(); ++n)
	{
		m_part_vel[n] = m_part_vel[n];
	}
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*m_part_pos.size(), m_part_pos.data());
}

GLuint ParticleHandler::get_vbo()
{
	return m_vbo;
}
