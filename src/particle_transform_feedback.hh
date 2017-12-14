#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

class ParticleTransformFeedback
{
public:
	ParticleTransformFeedback();
	~ParticleTransformFeedback();
	bool init(const glm::vec3 pos);
	void render(float dt, double time, glm::mat4 camera, glm::mat4 world, glm::vec3 eye);

private:
	bool m_isfirst;
	GLuint m_part_buffer[2];
	GLuint m_transform_feedback[2];
};

