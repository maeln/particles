#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 velocity;
layout(location = 2) in float ttl;

layout(std140) uniform camera
{
	vec4 eye;
	vec4 target;
	vec4 up;
	mat4 view;
	mat4 camera;
};

uniform float time;

smooth out float dist;
out float pttl;

void main()
{
	gl_Position = camera.camera * camera.view * vec4(position.xyz, 1.f);
	dist = distance(position, camera.eye);
	gl_PointSize = 6.0/dist;
	pttl = ttl;
}
