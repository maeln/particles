#pragma once

#include <glm/mat4x4.hpp>
#include <memory>

#include "src/graph/scene/scene_context.hh"
#include "src/graph/scene/scene_graph.hh"

#include "src/post/framebuffer/framebuffer_handler.hh"

class Pass {
public:
	Pass(std::shared_ptr<SceneGraph> scene)
	{
		m_scene = scene;
		m_fbo = 0;
	}

	Pass(std::shared_ptr<SceneGraph> scene, GLuint fbo)
	{
		m_scene = scene;
		m_fbo = fbo;
	}

	~Pass() {}

	void set_fbo(GLuint handler) { m_fbo = handler; }
	GLuint get_fbo() { return m_fbo; }

	void set_scene(std::shared_ptr<SceneGraph> scene) { m_scene = scene; }
	std::shared_ptr<SceneGraph> get_scene() { return m_scene; }

	void render(std::shared_ptr<SceneContext> ctx, glm::mat4 init_pos = glm::mat4())
	{
		if (m_fbo != 0) {
			GLuint fb_addr = m_fbh.get_framebuffer(m_fbo).addr;
			glBindFramebuffer(GL_FRAMEBUFFER, fb_addr);
		}

		m_scene->draw(ctx, init_pos);

		if (m_fbo != 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

private:
	std::shared_ptr<SceneGraph> m_scene;
	GLuint m_fbo;
	FramebufferHandler& m_fbh = FramebufferHandler::instance();
	;
};
