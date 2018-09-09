#pragma once

#include <memory>

#include "node/scene_node.hh"
#include "scene_context.hh"

class Scene : public SceneNode {
  public:
    Scene();
    ~Scene();

    void draw(std::shared_ptr<SceneContext> ctx);

  private:
};