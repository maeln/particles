#pragma once

#include <glm/mat4x4.hpp>
#include <memory>
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
    std::vector<std::shared_ptr<SceneNode>> &childs();

    void set_name(std::string);
    std::string get_name();
    void change_parent(std::shared_ptr<SceneNode> node);

    void add_child(std::shared_ptr<SceneNode> node);
    void rm_child(std::shared_ptr<SceneNode> node);

    glm::mat4x4 transformation();

    void rotate(float angle, glm::vec3 axis);
    void scale(glm::vec3 factors);
    void translate(glm::vec3 factors);

    void draw(std::shared_ptr<SceneContext>, glm::mat4x4);

  private:
    std::string m_name;
    glm::mat4x4 m_transformation;
    std::shared_ptr<SceneNode> m_parent;
    std::vector<std::shared_ptr<SceneNode>> m_childs;
};