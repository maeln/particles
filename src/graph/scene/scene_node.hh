#pragma once

#include <glm/mat4x4.hpp>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "src/object/drawable.hh"

class SceneNode : public Drawable {
public:
	SceneNode();
	SceneNode(std::string name);
	SceneNode(std::string name, std::shared_ptr<SceneNode> parent);
	virtual ~SceneNode();

	std::shared_ptr<SceneNode> parent();
	std::vector<std::shared_ptr<SceneNode>>& childs();

	void set_name(std::string);
	std::string get_name();
	void change_parent(std::shared_ptr<SceneNode> node);

	void add_child(std::shared_ptr<SceneNode> node);
	void rm_child(std::shared_ptr<SceneNode> node);
	std::optional<std::shared_ptr<SceneNode>> find_node(std::string name);

	void draw(std::shared_ptr<SceneContext>, glm::mat4x4);

private:
	std::string m_name;
	std::shared_ptr<SceneNode> m_parent;
	std::vector<std::shared_ptr<SceneNode>> m_childs;
};