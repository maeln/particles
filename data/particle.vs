#version 330

layout(location = 0) in vec4 position;

uniform float time;
uniform mat4 camera;

smooth out float dist;

void main()
{
	vec4 cameraPos = position + vec4(0.0, 0.0, -1.0, 0.0);
	gl_Position = camera * cameraPos;
	dist = sqrt(dot(camera * cameraPos, position));
	gl_PointSize = 15.0/dist;
}
