#pragma once

#include "src/object/camera/camera.hh"
#include <glm/mat4x4.hpp>
#include <memory>

class SceneContext {
  public:
    SceneContext() {}
    ~SceneContext() {}
    std::shared_ptr<Camera> activeCamera;
    glm::mat4 perspective;
    double t_time;
    double f_time;
    double v_width;
    double v_height;
};