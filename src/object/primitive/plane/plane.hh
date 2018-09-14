#pragma once

#include <GL/glew.h>

#include "src/graph/scene/scene_node.hh"
#include "src/object/mesh/primitive/plane_mesh.hh"
#include "src/shaders/shaderdb.hh"
#include <memory>
#include <vector>

class Plane : public SceneNode {
  public:
    Plane();
    ~Plane();

    void draw(std::shared_ptr<SceneContext>, glm::mat4x4);

  private:
    PlaneMesh m_mesh;
    ShaderDB &m_shaderdb = ShaderDB::instance();
    GLuint m_program;
};