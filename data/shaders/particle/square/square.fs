#version 330 core

uniform sampler2D particle;
uniform vec4 base;

uniform float time;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 eye;

in vec2 tex_coord;

in FS
{
	in vec4 pos;
};

out vec4 colour;

void main()
{
	float alpha = texture(particle, tex_coord).a;
	if (alpha == 0.0)
		discard;
	colour = vec4(base.rgba);
}
