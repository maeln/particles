#version 430 core

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;
layout(std140, binding = 0) buffer OffsetBuffer { vec4 offset[]; };

uniform float time;
uniform float dt;
uniform vec3 square;
uniform vec3 density;

float random(vec2 st) { return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123); }
float remap(float v, float l1, float h1, float l2, float h2) { return l2 + (v - l1) * (h2 - l2) / (h1 - l1); }

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise(in vec2 st)
{
	vec2 i = floor(st);
	vec2 f = fract(st);

	// Four corners in 2D of a tile
	float a = random(i);
	float b = random(i + vec2(1.0, 0.0));
	float c = random(i + vec2(0.0, 1.0));
	float d = random(i + vec2(1.0, 1.0));

	vec2 u = f * f * (3.0 - 2.0 * f);

	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

float fbm(in vec2 st)
{
	// Initial values
	float value = 0.0;
	float amplitude = 0.5;
	//
	// Loop of octaves
	for (int i = 0; i < 5; i++) {
		value += amplitude * noise(st);
		st *= 2.;
		amplitude *= .5;
	}
	return value;
}

void main(void)
{
	uint index = gl_GlobalInvocationID.x;
	vec4 off = offset[index];
	float id = float(index);
	float mp = density.x * density.y * density.z;

	float x = remap(mod(id, density.x), 0.0, density.x, 0.0, 1.0);
	float y = remap(floor(id / (density.x * density.z)), 0.0, density.y, 0.0, 1.0);
	float z = remap(mod(floor(id / density.z), density.z), 0.0, density.z, 0.0, 1.0);
	vec3 gl = vec3(x, y, z);

	vec2 uv = gl.xz * 4.0;

	float fbm1 = fbm(uv);
	vec2 q = vec2(0.0, 0.0);
	q.x = fbm(uv + 0.5 * time);
	q.y = fbm(uv + 0.6 * time);
	float fbm2 = fbm(uv + q);
	float fbm3 = clamp(2.0 * fbm(vec2(fbm1 + (time * 0.05), fbm2)) - 1.0, -1.0, 1.0);

	offset[index] = vec4(off.x, y + fbm3 * 4.0, off.z, 1.0);
}
