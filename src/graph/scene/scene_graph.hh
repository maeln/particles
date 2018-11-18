#pragma once

#include <GL/glew.h>

#include <glm/mat4x4.hpp>
#include <memory>

#include "scene_node.hh"
#include "src/post/framebuffer/framebuffer_handler.hh"

class SceneGraph : public SceneNode {
public:
	SceneGraph()
		: SceneNode()
	{
		m_clear = true;
	}

	SceneGraph(bool clear)
		: SceneNode()
	{
		m_clear = clear;
	}

	~SceneGraph() {}

	void set_clear(bool clear) { m_clear = clear; }
	bool get_clear() { return m_clear; }

	void draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 model)
	{
		if (m_clear) {
			// Clean Window.
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glClearColor(0.f, 0.f, 0.f, 0.f);
		}

		SceneNode::draw(ctx, model);
	}

private:
	bool m_clear;
};