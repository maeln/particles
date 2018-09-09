#pragma once

#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

#include "src/object/drawable.hh"

class SceneNode : public Drawable {
  public:
    SceneNode();
    SceneNode(std::shared_ptr<SceneNode> parent);
    virtual ~SceneNode();

    std::shared_ptr<SceneNode> parent();
    std::vector<std::shared_ptr<SceneNode>> &childs();

    void change_parent(std::shared_ptr<SceneNode> node);

    void add_child(std::shared_ptr<SceneNode> node);
    void rm_child(std::shared_ptr<SceneNode> node);

    glm::mat4x4 transformation();
    void push_transformation(glm::mat4x4 m);
    glm::mat4x4 pop_transformation();

    void draw(std::shared_ptr<SceneContext>, glm::mat4x4);

  private:
    std::vector<glm::mat4x4> m_mat_stack;
    std::shared_ptr<SceneNode> m_parent;
    std::vector<std::shared_ptr<SceneNode>> m_childs;
};