#version 330

layout (location = 0) in vec3 position;

uniform mat4 camera;
uniform mat4 world;

smooth out vec4 interpol;

void main()
{
	vec4 npos = world * vec4(position, 1.0);
	gl_Position = camera * npos;
    interpol = vec4(clamp(npos, 0.0, 1.0));
}
