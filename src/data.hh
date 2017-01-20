#pragma once

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glad/glad.h>

struct Point
{
	glm::vec4 pos;
	glm::vec4 dir;
	double ttl;
};

const std::vector<float> plane_vert = {
	-1.000000, -0.200000,  1.000000,
	 1.000000, -0.200000,  1.000000,
	-1.000000, -0.200000, -1.000000,
	 1.000000, -0.200000, -1.000000
};

const std::vector<int> plane_ind = {
	1, 2, 0,
	1, 3, 2
};
