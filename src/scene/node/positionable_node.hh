#include "scene_node.hh"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

class PositionableNode : public SceneNode {
  public:
    PositionableNode();
    ~PositionableNode();

    glm::mat4x4 transformation() {
	glm::mat4x4 identity = glm::mat4x4();
	for (size_t i = 0; i < mat_stack.size(); i++) {
	    identity *= mat_stack[i];
	}
	return identity;
    }

    void push_transformation(glm::mat4x4 m) { mat_stack.push_back(m); }

    glm::mat4x4 pop_transformation() {
	if (mat_stack.size() == 0) {
	    return glm::mat4x4();
	}
	glm::mat4x4 transfo = mat_stack.front();
	mat_stack.erase(mat_stack.begin());
	return transfo;
    }

  private:
    std::vector<glm::mat4x4> mat_stack;
};
