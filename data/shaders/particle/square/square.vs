#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec4 init;
layout(location = 3) in vec4 offset;

uniform float time;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 eye;

void main()
{
	gl_Position = position + init + offset;
}
