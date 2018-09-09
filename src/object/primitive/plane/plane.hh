#pragma once

#include "src/graph/scene/scene_node.hh"
#include "src/object/mesh/primitive/plane_mesh.hh"
#include "src/shader_handler.hh"
#include <memory>
#include <vector>

class Plane : public SceneNode {
  public:
    Plane(std::shared_ptr<Program> program);
    ~Plane();

    void draw(std::shared_ptr<SceneContext>, glm::mat4x4);

  private:
    PlaneMesh m_mesh;
    std::shared_ptr<Program> m_program;
};