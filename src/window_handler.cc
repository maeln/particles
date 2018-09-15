#include "window_handler.hh"

#include <algorithm>
#include <exception>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "src/object/particle/particle_handler.hh"
#include "src/object/primitive/plane/plane.hh"

#include "tools.hh"

WindowHandler::WindowHandler() {
    m_width = 1280;
    m_height = 768;

    /* Initialize the library */
    if (!glfwInit())
	throw new std::runtime_error("Unable to launch GLFW.");
    glfwSetErrorCallback(&WindowHandler::fw_error_callback);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    m_window = glfwCreateWindow(m_width, m_height, "Hello World", NULL, NULL);
    if (!m_window) {
	glfwTerminate();
	throw new std::runtime_error("Unable to display the window.");
    }

    // Setup callbacks;
    glfwSetFramebufferSizeCallback(m_window, &WindowHandler::fw_resize_callback);
    glfwSetKeyCallback(m_window, &WindowHandler::fw_keyboard_callback);
    glfwSetCursorPosCallback(m_window, &WindowHandler::fw_mouse_callback);

    // OpenGL context.
    glfwMakeContextCurrent(m_window);
    glewInit();
    glViewport(0, 0, m_width, m_height);

    m_mouse_dx = 0.0;
    m_mouse_dy = 0.0;

    m_ctx = std::shared_ptr<SceneContext>(new SceneContext());

    m_camera =
        std::shared_ptr<Camera>(new Camera(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f), 0.5f, 0.01f));
    m_ctx->activeCamera = m_camera;

    m_vsync = true;
}

WindowHandler::~WindowHandler() { glfwTerminate(); }

void WindowHandler::setup() {
    // OpenGL stuff
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.f, 1.f);
    glEnable(GL_DEPTH_CLAMP);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // VSync.
    glfwSwapInterval(m_vsync ? 1 : 0);

    m_perpective_matrix = glm::perspective(glm::radians(60.f), (float)m_width / (float)m_height, 0.1f, 10.f);
    m_ctx->perspective = m_perpective_matrix;

    m_max_part = 128 * 100;
    std::shared_ptr<ParticleHandler> particles(
        new ParticleHandler(m_max_part, 2.5, 2.5, glm::vec3(0.0, 0.5, 0.0), false, glm::vec3(41.0 / 255.0, 114.0 / 255.0, 200.0 / 255.0)));

    std::shared_ptr<Plane> plane(new Plane());

    /* Set up the scene */
    m_scene.add_child(plane);
    m_scene.add_child(particles);

    m_dt_acc = 0.0;
    m_frame_dt = 0.0;
    m_shader_reload_counter = 0.0;
}

void WindowHandler::rendering_loop() {
    double mouse_x, mouse_y;
    glfwGetCursorPos(m_window, &mouse_x, &mouse_y);
    double mouse_dx = 0.0;
    double mouse_dy = 0.0;
    while (!glfwWindowShouldClose(m_window)) {
	double start = glfwGetTime();

	// Clean Window.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 0.f);

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	    m_camera->process_key(GLFW_KEY_W, m_frame_dt);

	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	    m_camera->process_key(GLFW_KEY_S, m_frame_dt);

	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	    m_camera->process_key(GLFW_KEY_A, m_frame_dt);

	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	    m_camera->process_key(GLFW_KEY_D, m_frame_dt);

	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	    m_camera->process_key(GLFW_KEY_SPACE, m_frame_dt);

	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	    m_camera->process_key(GLFW_KEY_LEFT_SHIFT, m_frame_dt);

	mouse_dx = mouse_x;
	mouse_dy = mouse_y;
	glfwGetCursorPos(m_window, &mouse_x, &mouse_y);
	mouse_dx -= mouse_x;
	mouse_dy -= mouse_y;

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	    m_camera->process_mouse((float)mouse_dx, (float)mouse_dy, m_frame_dt);

	m_ctx->t_time = m_dt_acc;
	m_ctx->f_time = m_frame_dt;

	// Render stuff here.
	m_scene.draw(m_ctx, glm::mat4());

	glUseProgram(0);

	// End of the loop stuff.
	std::cout << std::fixed;
	std::cout.precision(0);
	std::cout << "\rfps: " << 1.0 / m_frame_dt << " | Point drawed: " << m_max_part;

	// Swapping buffers & polling events.
	glfwSwapBuffers(m_window);

	if (m_shader_reload_counter > 1.0) {
	    m_shaderdb.check_reload();
	    m_shader_reload_counter = 0.0;
	}

	double end = glfwGetTime();
	m_frame_dt = end - start;
	m_dt_acc += m_frame_dt;
	m_shader_reload_counter += m_frame_dt;

	glfwPollEvents();
    }
}

void WindowHandler::resize_callback(GLFWwindow *window, int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
    m_perpective_matrix = glm::perspective(glm::radians(60.f), (float)m_width / (float)m_height, 0.1f, 10.f);
    m_ctx->perspective = m_perpective_matrix;
}

void WindowHandler::error_callback(int error, const char *description) {
    std::cerr << "GLFW Error (code: " << error << "): " << description << std::endl;
}

void WindowHandler::keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
	m_vsync = !m_vsync;
	glfwSwapInterval(m_vsync ? 1 : 0);
    }
}

void WindowHandler::mouse_callback(GLFWwindow *window, double xpos, double ypos) {}
