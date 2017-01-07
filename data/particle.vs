#version 330

layout(location = 0) in vec4 position;
//smooth out vec2 pos;
uniform float time;
uniform mat4 camera;

void main()
{
	vec4 cameraPos = position + vec4(0.0, 0.0, -1.0, 0.0);
	gl_Position = camera * cameraPos;
	gl_PointSize = 5.0*dot(camera * cameraPos, position);
	// pos = position.xy;
}
