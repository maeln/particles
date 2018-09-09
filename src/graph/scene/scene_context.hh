#pragma once

#include "src/object/camera/camera.hh"
#include <glm/mat4x4.hpp>
#include <memory>

class SceneContext {
  public:
    std::shared_ptr<Camera> activeCamera;
    glm::mat4x4 perspective;
    double t_time;
    double f_time;
};