#pragma once
#include <memory>
#include <vector>

class SceneNode {
  public:
    SceneNode();
    SceneNode(std::shared_ptr<SceneNode> parent);
    ~SceneNode();

    std::shared_ptr<SceneNode> parent();
    std::vector<std::shared_ptr<SceneNode>> &childs();

    void change_parent(std::shared_ptr<SceneNode> node);

    void add_child(std::shared_ptr<SceneNode> node);
    void rm_child(std::shared_ptr<SceneNode> node);

  private:
    std::shared_ptr<SceneNode> m_parent;
    std::vector<std::shared_ptr<SceneNode>> m_childs;
};