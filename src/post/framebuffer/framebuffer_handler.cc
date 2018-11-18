#include "framebuffer_handler.hh"

#include <iostream>

Framebuffer FramebufferHandler::get_framebuffer(GLuint handler) { return m_fbs.at(handler); }

GLuint FramebufferHandler::create_full_framebuffer(double width, double height) { return create_framebuffer(width, height, 0); }

void FramebufferHandler::resize_framebuffer(GLuint handler, double width, double height)
{
	if (handler != 0) {
		GLuint addr = get_framebuffer(handler).addr;
		glDeleteFramebuffers(1, &addr);
		create_framebuffer(width, height, handler);
	}
}

GLuint FramebufferHandler::create_framebuffer(double width, double height, GLuint handler)
{
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Color attachement
	GLuint color_buffer;
	glGenTextures(1, &color_buffer);
	glBindTexture(GL_TEXTURE_2D, color_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "[ERR] Error while creating framebuffer !" << std::endl;
		throw new FramebufferCreationException();
	}

	Framebuffer fb(m_fb_count, fbo);
	fb.color = color_buffer;
	fb.depth_stencil = rbo;

	if (handler != 0) {
		fb.handler = handler;
	} else {
		m_fb_count += 1;
	}

	m_fbs[fb.handler] = fb;
	return fb.handler;
}