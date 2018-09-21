#pragma once

#include <GL/glew.h>

#include "src/graph/scene/scene_node.hh"
#include "src/object/mesh/primitive/quad_mesh.hh"
#include "src/object/positionable.hh"
#include "src/shaders/shaderdb.hh"
#include <memory>
#include <string>
#include <vector>


/**
 * A full screen quad.
 */
class FSQuad : public SceneNode, public Positionable {
public:
	FSQuad(std::string fs);
	~FSQuad(){};

	void draw(std::shared_ptr<SceneContext>, glm::mat4x4);

private:
	QuadMesh m_mesh;
	ShaderDB& m_shaderdb = ShaderDB::instance();
	GLuint m_program;
};