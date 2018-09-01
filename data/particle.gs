#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float time;
uniform mat4 camera;
uniform mat4 world;
uniform vec4 eye;

void main()
{
	vec4 center = world * gl_in[0].gl_Position;

	gl_Position = camera *  (center + vec4(0.1, 0.1, 0.0, 0.0));
	EmitVertex();

	gl_Position = camera *  (center + vec4(0.1, -0.1, 0.0, 0.0));
	EmitVertex();

	gl_Position = camera *  (center + vec4(-0.1, 0.1, 0.0, 0.0));
	EmitVertex();

	gl_Position = camera * (center + vec4(-0.1, -0.1, 0.0, 0.0));
	EmitVertex();

	EndPrimitive();
}
