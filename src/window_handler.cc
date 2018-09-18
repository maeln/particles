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
#include "src/object/primitive/fs_quad/fs_quad.hh"
#include "src/object/primitive/plane/plane.hh"

// ImGUI Stuff
#include "src/imgui/imgui.h"
#include "src/imgui/imgui_impl_glfw.h"
#include "src/imgui/imgui_impl_opengl3.h"

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

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui::StyleColorsDark();

    m_mouse_dx = 0.0;
    m_mouse_dy = 0.0;

    m_ctx = std::shared_ptr<SceneContext>(new SceneContext());
    m_ctx->v_width = m_width;
    m_ctx->v_height = m_height;

    m_camera =
        std::shared_ptr<Camera>(new Camera(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f), 0.5f, 0.01f));
    m_ctx->activeCamera = m_camera;

    m_scene_fbo = m_fb_handler.create_full_framebuffer(m_width, m_height);
    m_scene.set_fbo(m_scene_fbo);

    m_vsync = true;
}

WindowHandler::~WindowHandler() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

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
        new ParticleHandler(m_max_part, 2.0, 2.0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(41.0 / 255.0, 114.0 / 255.0, 200.0 / 255.0)));
    particles->set_name("parts1");

    std::shared_ptr<Plane> plane(new Plane());
    plane->translate(glm::vec3(0.0, -0.5, 0.0));
    plane->set_name("plane1");

    /* Set up the scene */
    m_scene.add_child(plane);
    m_scene.add_child(particles);

    /* Set up the fs quad */
    m_fs_scene.add_child(std::shared_ptr<FSQuad>(new FSQuad("data/shaders/post/post.fs")));

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
	glfwPollEvents();

	// GUI:
	std::shared_ptr<ParticleHandler> parts1 = std::dynamic_pointer_cast<ParticleHandler>(m_scene.find_node("parts1"));
	glm::vec3 parts_color = parts1->get_colour();
	float col[3] = {parts_color.r, parts_color.g, parts_color.b};

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Run info");

	bool changed = ImGui::Checkbox("VSync", &m_vsync);
	if (changed)
	    glfwSwapInterval(m_vsync ? 1 : 0);

	ImGui::Text("dt  : %.2fms", m_frame_dt * 1000.0);
	ImGui::Text("fps : %.2f", 1.0 / m_frame_dt);

	ImGui::ColorEdit3("Parts colour", col);
	parts1->set_colour(glm::vec3(col[0], col[1], col[2]));

	ImGui::End();

	double start = glfwGetTime();

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

	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	    glfwSetWindowShouldClose(m_window, GLFW_TRUE);

	mouse_dx = mouse_x;
	mouse_dy = mouse_y;
	glfwGetCursorPos(m_window, &mouse_x, &mouse_y);
	mouse_dx -= mouse_x;
	mouse_dy -= mouse_y;

	ImGuiIO &io = ImGui::GetIO();
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !io.WantCaptureMouse)
	    m_camera->process_mouse((float)mouse_dx, (float)mouse_dy, m_frame_dt);

	m_ctx->t_time = m_dt_acc;
	m_ctx->f_time = m_frame_dt;

	// Render stuff here.
	m_scene.draw(m_ctx, glm::mat4());

	// Render the framebuffer
	glBindTexture(GL_TEXTURE_2D, m_fb_handler.get_framebuffer(m_scene_fbo).color);
	m_fs_scene.draw(m_ctx, glm::mat4());

	glUseProgram(0);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Swapping buffers
	glfwSwapBuffers(m_window);

	if (m_shader_reload_counter > 1.0) {
	    m_shaderdb.check_reload();
	    m_shader_reload_counter = 0.0;
	}

	double end = glfwGetTime();
	m_frame_dt = end - start;
	m_dt_acc += m_frame_dt;
	m_shader_reload_counter += m_frame_dt;
    }
}

void WindowHandler::resize_callback(GLFWwindow *window, int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
    m_perpective_matrix = glm::perspective(glm::radians(60.f), (float)m_width / (float)m_height, 0.1f, 10.f);
    m_ctx->perspective = m_perpective_matrix;
    m_ctx->v_width = m_width;
    m_ctx->v_height = m_height;
    m_fb_handler.resize_framebuffer(m_scene_fbo, m_width, m_height);
}

void WindowHandler::error_callback(int error, const char *description) {
    std::cerr << "GLFW Error (code: " << error << "): " << description << std::endl;
}

void WindowHandler::keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {}

void WindowHandler::mouse_callback(GLFWwindow *window, double xpos, double ypos) {}
