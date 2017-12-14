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

bool ParticleTransformFeedback::init(const glm::vec3 pos) {

}

void ParticleTransformFeedback::render(float dt, double time, glm::mat4 camera, glm::mat4 world, glm::vec3 eye) {

}
