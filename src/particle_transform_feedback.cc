#include "particle_transform_feedback.hh"

struct Particle {
	float type;
	glm::vec3 position;
	glm::vec3 velocity;
	float ttl;
};

ParticleTransformFeedback::ParticleTransformFeedback()
{
}


ParticleTransformFeedback::~ParticleTransformFeedback()
{
}

bool ParticleTransformFeedback::init(const glm::vec3 pos) 
{
	Particle parts[100];
	parts[0].type = 0;
	parts[0].position = pos;
	parts[0].velocity = glm::vec3(0.0f, 0.0001f, 0.0f);
	parts[0].ttl = 0.0f;

	glGenTransformFeedbacks(2, m_transform_feedback);
	glGenBuffers(2, m_part_buffer);

	for (GLuint i = 0; i < 2; ++i) 
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transform_feedback[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_part_buffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(parts), parts, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_part_buffer[i]);
	}
}

void ParticleTransformFeedback::render(float dt, double time, glm::mat4 camera, glm::mat4 world, glm::vec3 eye) 
{

}
