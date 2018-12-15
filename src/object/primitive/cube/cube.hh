#pragma once

#include <GL/glew.h>
#include <memory>
#include <string>

#include "src/graph/scene/scene_node.hh"
#include "src/object/material/material.hh"
#include "src/object/mesh/primitive/cube_mesh.hh"
#include "src/object/positionable.hh"

class Cube : public SceneNode, public Positionable {
public:
	Cube(std::string vs, std::string fs);
	~Cube();

	void setMaterialData(std::shared_ptr<SceneContext>, glm::mat4x4);

	void draw(std::shared_ptr<SceneContext>, glm::mat4x4);

private:
	CubeMesh m_mesh;
	Material m_material;
	MaterialData m_uniforms;
	ShaderDB& m_shaderdb = ShaderDB::instance();
};