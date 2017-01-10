#version 330

layout (location = 0) in vec3 position;

uniform mat4 camera;

smooth out vec4 interpol;

void main()
{
	vec4 npos = vec4(position, 1.0);
	vec4 cameraPos = npos + vec4(0.0, 0.0, -1.0, 0.0);
	gl_Position = camera * cameraPos;
    interpol = vec4(clamp(npos, 0.0, 1.0));
}
