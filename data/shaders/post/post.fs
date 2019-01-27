#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform float time;
uniform float dt;
uniform vec2 resolution;

uniform sampler2D screenTexture;

#define ABERRATION_AMOUNT 1.1

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

// Quite literally stolen from Elysian by Logicoma
vec3 elysian_aberation(vec2 uv, float amount)
{
	vec3 color = vec3(0.0);
	const int taps = 100;

	for (int i = 0; i < taps; i++) {
		float f = float(i) / float(taps - 1);
		vec2 nv = uv * 2.0 - 1.0;
		float d = length(nv);
		vec2 scaled = nv / d * (d * (1.0 - f * amount * 0.1));
		scaled = mix(nv, scaled, d / length(vec2(1.0)));
		vec2 nuv = scaled * 0.5 + 0.5;
		vec3 tap = texture(screenTexture, nuv).rgb;
		color += tap * aberrationColor(f);
	}

	color /= float(taps) / 3.0;
	return color;
}

void main()
{
	vec2 uv = gl_FragCoord.xy / resolution.xy;

	// Chromab
	vec3 sum_color = vec3(0.0);
	sum_color += elysian_aberation(uv, ABERRATION_AMOUNT);

	// Fake film grain
	sum_color += ((0.5 - hash2(uv * cos(time))) / 20.0);

	// Gamma correction
	sum_color = pow(sum_color, vec3(2.2));

	FragColor = vec4(sum_color, 1.0);
}