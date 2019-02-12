#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "src/graph/scene/scene_node.hh"
#include "src/object/positionable.hh"
#include "src/shaders/shaderdb.hh"

class SquareEmitter : public SceneNode, public Positionable {
public:
	SquareEmitter(glm::vec3 square, glm::vec3 density, glm::vec4 color);
	~SquareEmitter();

	void draw(std::shared_ptr<SceneContext>, glm::mat4x4);

	void set_colour(glm::vec4 col);
	glm::vec4 get_colour();

private:
	void update_particles(std::shared_ptr<SceneContext> ctx);

	ShaderDB& m_shadb = ShaderDB::instance();

	GLuint m_nbparts;
	GLuint m_texture;

	glm::vec3 m_square;
	glm::vec3 m_density;
	glm::vec4 m_color;

	GLuint m_initializer;
	GLuint m_updater;
	GLuint m_material;

	GLuint m_init;
	GLuint m_offset;
	GLuint m_point;
	GLuint m_vao;
};
