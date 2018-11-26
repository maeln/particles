#pragma once

#include "src/math/matrix_stack.hh"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

class Positionable {
public:
	Positionable(){};
	~Positionable(){};

	glm::mat4 transform()
	{
		return m_stack.top();
	};

	glm::mat4 reset_transform()
	{
		m_stack.clear();
		return glm::mat4();
	};

	void commit_transform()
	{
		m_stack.flatten();
	};

	void rotate(glm::vec3 axis, float angle)
	{
		m_stack.push(glm::rotate(glm::mat4(), angle, axis));
	};

	void translate(glm::vec3 axis)
	{
		m_stack.push(glm::translate(glm::mat4(), axis));
	};

	void scale(glm::vec3 axis)
	{
		m_stack.push(glm::scale(glm::mat4(), axis));
	};

private:
	MatrixStack m_stack;
};
