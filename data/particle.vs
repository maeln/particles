#version 330

layout(location = 0) in vec4 position;

uniform float time;
uniform mat4 camera;
uniform mat4 world;
uniform vec4 eye;

smooth out float dist;

void main()
{
	vec4 cameraPos = world * position;
	gl_Position = camera * cameraPos;
	dist = distance(position, eye);
	gl_PointSize = 10/dist;
}
