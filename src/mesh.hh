#pragma once

#include <glad/glad.h>

/* Generic Mesh interface */
class Mesh
{
public:
	/**
	 * Put the data of the mesh inside a VBO to be ready for drawing.
	 */
	virtual void upload_to_gpu() = 0;

	/**
	 * Remove the data from the GPU memory.
	 * If upload_to_gpu() has not been called, this function do nothing.
	 */
	virtual void remove_from_gpu() = 0;

	/**
	 * Abstract function ton draw the mesh.
	 * Should simply bind the VBO, set the vertex attribute arry / pointer
	 * and call glDraw*.
	 *
	 * This function should do nothing if upload_to_gpu() has not been called before.
	 */
	virtual void draw() = 0;
};
