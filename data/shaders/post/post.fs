#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec2 resolution;

uniform sampler2D screenTexture;

#define PI 3.141592

// Anti fish eye lens distortion from: https://www.shadertoy.com/view/4s2GRR
vec2 antiFishEye(in float amount, in vec2 fragCoord) {
    vec2 uv = fragCoord.xy / resolution.xy;
    float prop = resolution.x / resolution.y;
    vec2 center = vec2(0.5, 0.5 / prop);
    vec2 d = uv - center;
    float r = sqrt(dot(d, d));
    float power = (2.0 * PI / (2.0 * sqrt(dot(center, center)))) * amount;

    float bind;
    if (power > 0.0) {
	bind = sqrt(dot(center, center));
    } else {
	if (prop < 1.0) {
	    bind = center.x;
	} else {
	    bind = center.y;
	}
    }
    return center + normalize(d) * atan(r * -power * 10.0) * bind / atan(-power * bind * 10.0);
}

void main() {
    vec2 uv = gl_FragCoord.xy / resolution.xy;

    // lens distortion with chromatic abberation
    float power = 0.01;
    vec4 r = texture(screenTexture, antiFishEye(0.001 + power, gl_FragCoord.xy));
    vec4 g = texture(screenTexture, antiFishEye(0.005 + power, gl_FragCoord.xy));
    vec4 b = texture(screenTexture, antiFishEye(0.010 + power, gl_FragCoord.xy));

    FragColor = vec4(r.r, g.g, b.b, 1.0);
}