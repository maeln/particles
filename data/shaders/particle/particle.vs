#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in float ttl;

uniform float time;
uniform mat4 camera;
uniform mat4 world;
uniform vec4 eye;

void main()
{
	gl_Position = position;
}
