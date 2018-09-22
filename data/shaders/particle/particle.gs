#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 tex_coord;

uniform float time;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec4 eye;

out FS
{
	out vec4 pos;
}
fs;

void main()
{
	fs.pos = model * gl_in[0].gl_Position;
	vec4 center = view * model * gl_in[0].gl_Position;
	const vec2 vert_coordinates[] = vec2[](vec2(-1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, -1.0), vec2(1.0, 1.0));
	const vec2 tex_coordinates[] = vec2[](vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f));
	for (int i = 0; i < 4; i++) {
		gl_Position = projection * (center + vec4(vert_coordinates[i] * 0.01, 0.0f, 0.0f));
		tex_coord = tex_coordinates[i];
		EmitVertex();
	}
}
