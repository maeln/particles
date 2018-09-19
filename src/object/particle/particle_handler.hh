#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <memory>
#include <random>
#include <vector>

#include "src/graph/scene/scene_node.hh"
#include "src/shaders/shaderdb.hh"

class ParticleHandler : public SceneNode {
public:
	ParticleHandler(GLuint nb_particule, float ttl_particule, float delta_ttl, glm::vec3 start_point, glm::vec3 base_colour);
	~ParticleHandler();

	void update_particules(float time, float dt, float speed_factor = 1.f);
	void draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 model);

	void set_colour(glm::vec3 col);
	glm::vec3 get_colour();

private:
	GLuint m_vbo_pos;
	GLuint m_vbo_vel;
	GLuint m_vbo_ttl;
	GLuint m_vao;

	GLuint m_max_part;
	float m_ttl;
	float m_dttl;
	glm::vec3 m_pstart;
	GLuint m_texture;

	glm::vec3 m_base_colour;

	std::default_random_engine m_randgen;
	std::uniform_real_distribution<float> m_uniform;

	std::vector<float> m_part_pos;
	std::vector<float> m_part_vel;
	std::vector<float> m_part_ttl;

	ShaderDB& m_shaderdb = ShaderDB::instance();
	GLuint m_visual_program;
	GLuint m_compute_program;

	bool m_compute_supported;

	float* m_data;
};
