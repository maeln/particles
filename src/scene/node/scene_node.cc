#include "scene_node.hh"

SceneNode::SceneNode() { m_parent = nullptr; }

SceneNode::SceneNode(std::shared_ptr<SceneNode> parent) { m_parent = parent; }

SceneNode::~SceneNode() {}

std::shared_ptr<SceneNode> SceneNode::parent() { return m_parent; }

std::vector<std::shared_ptr<SceneNode>> &SceneNode::childs() { return m_childs; }

void SceneNode::change_parent(std::shared_ptr<SceneNode> node) { m_parent = node; }

void SceneNode::add_child(std::shared_ptr<SceneNode> node) { m_childs.push_back(node); }

void SceneNode::rm_child(std::shared_ptr<SceneNode> node) {
    int pos = -1;
    for (int i = 0; i < m_childs.size(); i++) {
	if (m_childs.at(i) == node) {
	    pos = i;
	    break;
	}
    }
    m_childs.erase(m_childs.begin() + pos);
}