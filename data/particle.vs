#version 330

layout(location = 0) in vec4 position;

uniform float time;
uniform mat4 camera;

smooth out float dist;

void main()
{
	vec4 cameraPos = position + vec4(0.0, 0.0, -1.0, 0.0);
	gl_Position = camera * cameraPos;
	gl_PointSize = 5.0*dot(camera * cameraPos, position);
	dist = dot(camera*cameraPos, position);
	// pos = position.xy;
}
