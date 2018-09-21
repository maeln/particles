#pragma once

#include <GL/glew.h>

#include <glm/mat4x4.hpp>
#include <memory>

#include "src/graph/scene/scene_context.hh"
#include "src/shaders/shaderdb.hh"

/** TODO:
 * Add a render target.
 */
class Drawable {
public:
	virtual void draw(std::shared_ptr<SceneContext>, glm::mat4x4) = 0;

	void set_program(GLuint program)
	{
		m_program = program;
	};

	GLuint get_program()
	{
		return m_program;
	}

protected:
	GLuint m_program = 0;
	ShaderDB& m_shaderdb = ShaderDB::instance();
};