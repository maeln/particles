#pragma once

#include <glad/glad.h>

/* Generic Mesh interface */
class Mesh
{
public:
	/**
	 * Abstract function ton draw the mesh.
	 * Should simply bind the VBO, set the vertex attribute arry / pointer
	 * and call glDraw*.
	 * For safety purposes, should also unbind everything at the end.
	 */
	virtual void draw();
};
