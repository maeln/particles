#pragma once

#include <vector>
#include <random>
#include <glad/glad.h>

class ParticleHandler
{
public:
	ParticleHandler(GLuint nb_particule, float ttl_particule, float delta_ttl);
	~ParticleHandler();
	
	void update_particules();
	GLuint get_vbo();

private:
	GLuint m_vbo;
	GLuint m_max_part;
	float m_part_ttl;
	std::default_random_engine m_randgen;
	std::uniform_real_distribution<double> m_uniform(-1.0, 1.0);
	
	std::vector<float> m_part_pos;
	std::vector<float> m_part_vel;
	std::vector<float> m_part_ttl;
};
