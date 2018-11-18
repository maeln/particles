#version 330 core

uniform sampler2D particle;
uniform vec4 base;

uniform float time;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 eye;

in vec4 pos;
in vec2 tex_coord;

out vec4 colour;

void main()
{
	float alpha = texture(particle, tex_coord).a;

	float rx = sin(pos.x);
	float ry = cos(pos.z);
	if (rx + ry < 0.0) {
		discard;
	}

	if (sin(pos.y * 4.0 + (time * 5.0)) < 0.0) {
		discard;
	}

	if (alpha == 0.0)
		discard;
	colour = vec4(base.rgba);
}
