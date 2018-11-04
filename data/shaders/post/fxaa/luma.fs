#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
	vec3 color = texture(screenTexture, TexCoords).xyz;
	float lum = dot(color, vec3(0.299, 0.587, 0.114));
	FragColor = vec4(color, lum);
}
