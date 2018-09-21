#pragma once

#include <deque>
#include <glm/mat4x4.hpp>

class MatrixStack {
public:
	MatrixStack()
	{
		m_top = glm::mat4();
	};
	~MatrixStack(){};

	glm::mat4 pop()
	{
		glm::mat4 r = m_stack.front();
		m_stack.erase(m_stack.begin());
		recompute();
		return r;
	};

	void push(glm::mat4 mat)
	{
		m_stack.push_front(mat);
		m_top = glm::mat4();
		recompute();
	};

	glm::mat4 top()
	{
		return m_top;
	};

	void clear()
	{
		m_top = glm::mat4();
		m_stack.clear();
	};

private:
	void recompute()
	{
		m_top = glm::mat4();
		for (glm::mat4 m : m_stack) {
			m_top *= m;
		}
	};

	glm::mat4 m_top;
	std::deque<glm::mat4> m_stack;
};
