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

glm::mat4x4 SceneNode::transformation() {
    glm::mat4x4 identity = glm::mat4x4();
    for (size_t i = 0; i < m_mat_stack.size(); i++) {
	identity *= m_mat_stack[i];
    }
    return identity;
}

void SceneNode::push_transformation(glm::mat4x4 m) { m_mat_stack.push_back(m); }

glm::mat4x4 SceneNode::pop_transformation() {
    if (m_mat_stack.size() == 0) {
	return glm::mat4x4();
    }
    glm::mat4x4 transfo = m_mat_stack.front();
    m_mat_stack.erase(m_mat_stack.begin());
    return transfo;
}

void SceneNode::draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 transfo) {
    glm::mat4x4 final_transfo = transfo * transformation();
    for (size_t i = 0; i < m_childs.size(); i++) {
	m_childs[i]->draw(ctx, final_transfo);
    }
}
