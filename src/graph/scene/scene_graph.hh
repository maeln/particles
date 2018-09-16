#pragma once

#include <GL/glew.h>

#include <glm/mat4x4.hpp>
#include <memory>

#include "scene_node.hh"
#include "src/post/framebuffer/framebuffer_handler.hh"

class SceneGraph : public SceneNode {
  public:
    SceneGraph() : SceneNode() { m_fbo = 0; };
    ~SceneGraph(){};

    void set_fbo(GLuint handler) { m_fbo = handler; }

    void draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 model) {
	// Clean Window.
	if (m_fbo != 0) {
	    GLuint fb_addr = m_fb_handler.get_framebuffer(m_fbo).addr;
	    glBindFramebuffer(GL_FRAMEBUFFER, fb_addr);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	SceneNode::draw(ctx, model);

	if (m_fbo != 0) {
	    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
    };

  private:
    GLuint m_fbo;
    FramebufferHandler &m_fb_handler = FramebufferHandler::instance();
};