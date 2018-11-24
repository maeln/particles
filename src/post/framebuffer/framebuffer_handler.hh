#pragma once

#include <GL/glew.h>

#include <exception>
#include <map>

// Framebuffer flags
const unsigned char RGBA_FRAMEBUFFER = 0b0000'0001;
const unsigned char FLOAT_FRAMEBUFFER = 0b0000'0010;
const unsigned char DEPTH_STENCIL_RENDERBUFFER = 0b0000'0100;

class FramebufferCreationException : public std::exception {
	const char* what() const throw() { return "Framebuffer Creation Exception"; }
};

class Framebuffer {
public:
	Framebuffer(){};
	Framebuffer(GLuint h, GLuint a)
	{
		handler = h;
		addr = a;
	};

	GLuint handler;
	GLuint addr;

	GLuint color;
	GLuint depth_stencil;
};

class FramebufferHandler {
public:
	static FramebufferHandler& instance()
	{
		static FramebufferHandler instance;
		return instance;
	};
	~FramebufferHandler()
	{
		for (auto fb : m_fbs) {
			glDeleteFramebuffers(1, &fb.second.addr);
		}
	};

	Framebuffer get_framebuffer(GLuint handler);
	void resize_framebuffer(GLuint handler, double width, double height);

	/** Create a "full" framebuffer (With depth, stencil and color attachement) */
	GLuint create_full_framebuffer(double width, double height);

private:
	FramebufferHandler() { m_fb_count = 1; }; // 0 is reserved;
	FramebufferHandler(const FramebufferHandler&);
	void operator=(const FramebufferHandler&);

	GLuint create_framebuffer(double width, double height, const unsigned char flags, GLuint handler);

	GLuint m_fb_count;
	std::map<GLuint, Framebuffer> m_fbs;
};
