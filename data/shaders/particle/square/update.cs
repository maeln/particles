#version 430 core

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;
layout(std140, binding = 0) buffer OffsetBuffer { vec4 offset[]; };

uniform vec3 square;
uniform vec3 density;

float random(vec2 st) { return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123); }

void main(void)
{
	uint id = gl_GlobalInvocationID.x;
	vec4 off = offset[id];

	offset[id] = off;
}
