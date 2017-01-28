#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 fcolour;

uniform float time;
uniform mat4 camera;
uniform mat4 world;
uniform vec4 eye;

smooth out float dist;
out vec3 vcolour;

void main()
{
	vec4 cameraPos = world * position;
	gl_Position = camera * cameraPos;
	dist = distance(position, eye);
	gl_PointSize = 13.0/dist;
	vcolour = fcolour;
}
