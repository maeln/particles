#include "scene_node.hh"

#include <glm/gtc/matrix_transform.hpp>

SceneNode::SceneNode()
{
	m_name = "";
	m_parent = nullptr;
}

SceneNode::SceneNode(std::string name)
{
	m_name = name;
	m_parent = nullptr;
}

SceneNode::SceneNode(std::string name, std::shared_ptr<SceneNode> parent)
{
	m_name = name;
	m_parent = parent;
}

SceneNode::~SceneNode() {}

void SceneNode::set_name(std::string name) { m_name = name; }
std::string SceneNode::get_name() { return m_name; }

std::shared_ptr<SceneNode> SceneNode::parent() { return m_parent; }

std::vector<std::shared_ptr<SceneNode>>& SceneNode::childs() { return m_childs; }

void SceneNode::change_parent(std::shared_ptr<SceneNode> node) { m_parent = node; }

void SceneNode::add_child(std::shared_ptr<SceneNode> node) { m_childs.push_back(node); }

void SceneNode::rm_child(std::shared_ptr<SceneNode> node)
{
	uint32_t pos = -1;
	for (uint32_t i = 0; i < m_childs.size(); i++) {
		if (m_childs.at(i) == node) {
			pos = i;
			break;
		}
	}
	m_childs.erase(m_childs.begin() + pos);
}

std::optional<std::shared_ptr<SceneNode>> SceneNode::find_node(std::string name)
{
	// Look a first level child first
	for (auto node : m_childs) {
		if (node->get_name() == name)
			return std::optional<std::shared_ptr<SceneNode>>(node);
	}

	// If we didn't find, we do a recursive find
	for (auto node : m_childs) {
		auto res = node->find_node(name);
		if (res)
			return std::optional<std::shared_ptr<SceneNode>>(res);
	}

	return {};
}

void SceneNode::draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 transfo)
{
	for (size_t i = 0; i < m_childs.size(); i++) {
		m_childs[i]->draw(ctx, transfo);
	}
}
