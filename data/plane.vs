#version 330

layout (location = 0) in vec3 position;

layout(std140) uniform camera
{
	vec4 eye;
	vec4 target;
	vec4 up;
	mat4 view;
	mat4 camera;
};

smooth out vec4 interpol;

void main()
{
	vec4 npos = camera.world * vec4(position, 1.0);
	gl_Position = camera.camera * npos;
    interpol = vec4(clamp(vec4(position, 1.0), 0.0, 1.0));
}
