#version 330

layout(location = 0) in vec4 position;

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

void main()
{
	vec4 cameraPos = camera.world * position;
	gl_Position = camera.camera * cameraPos;
	dist = distance(position, camera.eye);
	gl_PointSize = 5.0/dist;
}
