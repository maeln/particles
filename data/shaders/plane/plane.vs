#version 330

layout(location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

smooth out vec4 interpol;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	interpol = vec4(clamp(position, 0.0, 1.0), 1.0);
}
