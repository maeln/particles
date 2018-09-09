#pragma once

#include "node/camera/camera.hh"
#include <memory>

class SceneContext {
    std::shared_ptr<Camera> activeCamera;
    std::shared_ptr<double> t_time;
    std::shared_ptr<double> f_time;
}