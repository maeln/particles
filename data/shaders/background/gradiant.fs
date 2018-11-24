#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform float time;
uniform float dt;
uniform vec2 resolution;

uniform sampler2D screenTexture;

// http://www.iquilezles.org/www/articles/functions/functions.htm
float impulse(float k, float x)
{
	float h = k * x;
	return h * exp(1.0 - h);
}

float expStep(float x, float k, float n)
{
	return exp(-k * pow(x, n));
}

void main()
{
	// Normalized pixel coordinates (from 0 to 1)
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	// Gradient color
	vec3 col1 = vec3(4, 11, 11) / 255.0;
	vec3 col2 = vec3(13, 59, 78) / 255.0;

	// Center of the gradient
	vec2 center = vec2(0.70, 0.45);

	// Gradiant
	vec3 grad = mix(col1, col2, expStep(distance(uv, center), 5.0, 2.0));

	// Output to screen
	FragColor = vec4(grad, 1.0);
}
