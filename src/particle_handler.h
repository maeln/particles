#pragma once

#include <vector>
#include <random>
#include <glad/glad.h>

class ParticleHandler
{
public:
	ParticleHandler(GLuint nb_particule, float ttl_particule, float delta_ttl);
	~ParticleHandler();
	
	void update_particules(float dt, float speed_factor=1.f);
	GLuint get_vbo();

private:
	GLuint m_vbo;
	GLuint m_max_part;
	float m_ttl;
	float m_dttl;
	std::default_random_engine m_randgen;
	std::uniform_real_distribution<double> m_uniform;
	
	std::vector<float> m_part_pos;
	std::vector<float> m_part_vel;
	std::vector<float> m_part_ttl;
};
