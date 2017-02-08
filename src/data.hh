#pragma once

#include <glad/glad.h>

const float plane_vert[] =
{
	-1.000000, -0.200000,  1.000000,
	1.000000, -0.200000,  1.000000,
	-1.000000, -0.200000, -1.000000,
	1.000000, -0.200000, -1.000000
};

const GLuint plane_ind[] =
{
	1, 2, 0,
	1, 3, 2
};
