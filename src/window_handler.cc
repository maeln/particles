#include "window_handler.h"

WindowHandler::WindowHandler(int width, int height)
{
	m_width = width;
	m_height = height;
	
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwSetErrorCallback(WindowHandler::error_callback);
	
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    
    /* Create a windowed mode window and its OpenGL context */
    m_window = glfwCreateWindow(resx, resy, "Hello World", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        return -1;
    }
    
    // Setup callbacks;
    glfwSetFramebufferSizeCallback(m_window, WindowHandler::framebuffer_size_callback);
    glfwSetKeyCallback(m_window, WindowHandler::key_callback);
    glfwSetCursorPosCallback(m_window, WindowHandler::cursor_position_callback);
    
    // OpenGL context.
    glfwMakeContextCurrent(m_window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glViewport(0, 0, m_width, m_height);
}

WindowHandler::~WindowHandler()
{
	glfwTerminate();
}

void WindowHandler::setup()
{
	// OpenGL stuff
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
	glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.f, 1.f);
	glEnable(GL_DEPTH_CLAMP);
	
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WindowHandler::rendering_loop()
{
	while (!glfwWindowShouldClose(m_window))
    {
		// Render stuff here;
		
		// Swapping buffers & polling events.
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void WindowHandler::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, width, height);
}

void WindowHandler::error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error (code: " << error << "): " << description << std::endl;
}

void WindowHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_FALSE);
}

void WindowHandler::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
}
