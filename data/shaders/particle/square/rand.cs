#version 430 core

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;
layout(std140, binding = 0) buffer InitBuffer { vec4 init[]; };

uniform vec3 square;
uniform vec3 density;

float random(vec2 st) { return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123); }

float remap(float v, float l1, float h1, float l2, float h2) { return l2 + (v - l1) * (h2 - l2) / (h1 - l1); }

void main(void)
{
	uint index = gl_GlobalInvocationID.x;
	float id = float(index);
	float mp = density.x * density.y * density.z;

	float x = remap(mod(id, density.x), 0.0, density.x, 0.0, square.x);
	float y = remap(floor(id / (density.x * density.z)), 0.0, density.y, 0.0, square.y);
	float z = remap(mod(floor(id / density.z), density.z), 0.0, density.z, 0.0, square.z);
	vec3 gl = vec3(x, y, z);

	// for now, just put them randomly
	float nx = remap(random(vec2(x, density.x)), 0.0, 1.0, 0.0, square.x);
	float ny = remap(random(vec2(y, density.y)), 0.0, 1.0, 0.0, square.y);
	float nz = remap(random(vec2(z, density.z)), 0.0, 1.0, 0.0, square.z);
	vec3 pos = vec3(nx, ny, nz);

	init[index] = vec4(pos, 1.0);
}
