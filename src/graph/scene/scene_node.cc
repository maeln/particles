#include "scene_node.hh"

#include <glm/gtc/matrix_transform.hpp>

SceneNode::SceneNode() {
    m_name = "";
    m_parent = nullptr;
}

SceneNode::SceneNode(std::string name) {
    m_name = name;
    m_parent = nullptr;
}

SceneNode::SceneNode(std::string name, std::shared_ptr<SceneNode> parent) {
    m_name = name;
    m_parent = parent;
}

SceneNode::~SceneNode() {}

void SceneNode::set_name(std::string name) { m_name = name; }
std::string SceneNode::get_name() { return m_name; }

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

glm::mat4x4 SceneNode::transformation() { return m_transformation; }

void SceneNode::rotate(float angle, glm::vec3 axis) { m_transformation = glm::rotate(m_transformation, angle, axis); }

void SceneNode::scale(glm::vec3 factors) { m_transformation = glm::scale(m_transformation, factors); }

void SceneNode::translate(glm::vec3 factors) { m_transformation = glm::translate(m_transformation, factors); }

void SceneNode::draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 transfo) {
    glm::mat4x4 final_transfo = transfo * transformation();
    for (size_t i = 0; i < m_childs.size(); i++) {
	m_childs[i]->draw(ctx, final_transfo);
    }
}
