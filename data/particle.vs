#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 velocity;
layout(location = 2) in float ttl;

uniform float time;
uniform mat4 camera;
uniform mat4 world;
uniform vec4 eye;

smooth out float dist;
out float pttl;

void main()
{
	vec4 adj_pos = vec4(position, 1.0);
	vec4 cameraPos = world * adj_pos;
	gl_Position = camera * cameraPos;
	dist = distance(adj_pos, eye);
	gl_PointSize = 6.0/dist;
	pttl = ttl;
}
