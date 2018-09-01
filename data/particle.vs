#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 velocity;
layout(location = 2) in float ttl;

uniform float time;
uniform mat4 camera;
uniform mat4 world;
uniform vec4 eye;

smooth out float dist;
out float pttl;

void main()
{
	gl_Position = position;
	/*
	dist = distance(position, eye);
	gl_PointSize = 10.0/dist;
	pttl = ttl;
	*/
}
