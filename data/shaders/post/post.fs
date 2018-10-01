#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform float time;
uniform float dt;
uniform vec2 resolution;

uniform sampler2D screenTexture;

#define MAX_DIST 1.5
#define MIN_DIST 1.1
#define ITER_DIST 20

#define PI 3.141592
#define saturate(x) (clamp((x), 0.0, 1.0))

float hash2(vec2 n)
{
	return fract(sin(dot(n.xy, vec2(12.9898, 78.233))) * 43758.5453);
}
vec2 remap(vec2 t, vec2 a, vec2 b) { return clamp((t - a) / (b - a), 0.0, 1.0); }

// RGB ramp stolen from Ferris
vec3 aberrationColor(float f)
{
	f = f * 3.0 - 1.5;
	return saturate(vec3(-f, 1.0 - abs(f), f));
}

vec2 brownConradyDistortion(vec2 uv, float factor)
{
	uv = uv * 1.9 - 0.95;

	// positive values of K1 give barrel distortion, negative give pincushion
	float barrelDistortion1 = 0.1 * factor; // K1 in text books
	float barrelDistortion2 = -0.025 * factor; // K2 in text books
	float r2 = uv.x * uv.x + uv.y * uv.y;
	uv *= 0.99 + barrelDistortion1 * r2 + barrelDistortion2 * r2 * r2;

	// tangential distortion (due to off center lens elements)
	// is not modeled in this function, but if it was, the terms would go here
	return uv * 0.5 + 0.5;
}

vec2 distort(vec2 uv, float f, float min_d, float max_d) { return brownConradyDistortion(uv, mix(min_d, max_d, f)); }

vec3 frame(vec2 uv) { return pow(texture(screenTexture, uv).rgb, vec3(2.2)); }

void main()
{
	vec2 uv = gl_FragCoord.xy / resolution.xy;
	vec2 d = brownConradyDistortion(uv, MAX_DIST);
	if (d.x > 1.0 || d.x < 0.0 || d.y > 1.0 || d.y < 0.0) {
		discard;
	}

	const float step_size = 1.0 / (float(ITER_DIST) - 1.0);
	float t = step_size * hash2(uv * sin(time)); // We pseudo randomize the step to have some dithering pattern.

	vec3 sum_color = vec3(0.0);
	vec3 sum_weight = vec3(0.0);
	for (int i = 0; i < ITER_DIST; ++i) {
		vec3 weight = aberrationColor(t);
		sum_weight += weight;
		sum_color += weight * frame(distort(uv, t, MIN_DIST, MAX_DIST));
		t += step_size;
	}

	sum_color /= sum_weight;
	sum_color = pow(sum_color, vec3(1.0 / 2.2));
	sum_color += hash2(uv * sin(time)) / 255.0;

	FragColor = vec4(sum_color, 1.0);
}