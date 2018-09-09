#pragma once

#include "src/graph/scene/scene_context.hh"
#include <glm/mat4x4.hpp>
#include <memory>

/** TODO:
 * Add a render target.
 */
class Drawable {
  public:
    virtual void draw(std::shared_ptr<SceneContext>, glm::mat4x4) = 0;
};