#version 330 core

uniform sampler2D part_texture;
uniform vec4 part_colour;
uniform float time;

in vec2 tex_coord;

in FS
{
	in vec4 pos;
};

out vec4 colour;

void main()
{
	float alpha = texture(part_texture, tex_coord).a;
	if (alpha == 0.0)
		discard;
	colour = vec4(part_colour.rgb, 1.0 - step(-0.9, sin(pos.z * 8.0 + time * 4.0)));
}
