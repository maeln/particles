#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <map>
#include <memory>
#include <string>

#include "src/graph/scene/scene_context.hh"
#include "src/graph/scene/scene_graph.hh"
#include "src/object/camera/camera.hh"
#include "src/shaders/shaderdb.hh"

#include <GLFW/glfw3.h>

class WindowHandler {
  public:
    static WindowHandler &instance() {
	static WindowHandler instance;
	return instance;
    }
    ~WindowHandler();

    void setup();
    void rendering_loop();

  private:
    WindowHandler();
    WindowHandler(WindowHandler const &);
    void operator=(WindowHandler const &);

    static void fw_resize_callback(GLFWwindow *window, int width, int height) { instance().resize_callback(window, width, height); };
    static void fw_error_callback(int error, const char *description) { instance().error_callback(error, description); };
    static void fw_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	instance().keyboard_callback(window, key, scancode, action, mods);
    };
    static void fw_mouse_callback(GLFWwindow *window, double xpos, double ypos) { instance().mouse_callback(window, xpos, ypos); };

    void resize_callback(GLFWwindow *window, int width, int height);
    void error_callback(int error, const char *description);
    void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void mouse_callback(GLFWwindow *window, double xpos, double ypos);

    // Private vars.
    GLFWwindow *m_window;
    GLuint m_width;
    GLuint m_height;
    bool m_vsync;

    double m_mouse_dx;
    double m_mouse_dy;

    double m_frame_dt;
    double m_dt_acc;
    double m_prev_t;

    ShaderDB &m_shaderdb = ShaderDB::instance();

    std::shared_ptr<Camera> m_camera;
    glm::mat4 m_perpective_matrix;
    SceneGraph m_scene;

    std::shared_ptr<SceneContext> m_ctx;

    GLuint m_max_part;
};
