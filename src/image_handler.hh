#pragma once

#include <GL/glew.h>
#include <string>

class ImageHandler {
public:
	ImageHandler();
	~ImageHandler();

	static GLuint load_texture(std::string filename);
};
