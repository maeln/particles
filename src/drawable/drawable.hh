#pragma once

#include "src/scene/scene_context.hh"

/** TODO:
 * Add a render target.
 */
class Drawable {
  public:
    virtual void draw(SceneContext *);
}