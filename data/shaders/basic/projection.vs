#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 projected_position;
out vec4 transposed_normal;

void main()
{
	vec4 pos = projection * view * model * vec4(position, 1.0);
	gl_Position = pos;

	projected_position = pos;
	transposed_normal = normalize(transpose(projection) * vec4(normal, 1.0));
}
