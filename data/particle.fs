#version 330

uniform sampler2D texture;

in vec2 tex_coord;

in FS {
    in float dist;
    in float ttl;
};

out vec4 colour;

void main() {
    // colour = vec4(mix(1.0, 0.0, 1.0 - (2.5 - pttl)), 0.0, mix(1.0, 0.0, 2.5 - pttl), 1.0);
    vec4 texColour = texture2D(texture, tex_coord); // texture2D(texture, gl_FragCoord.xy / vec2(1280.0, 768.0)).rgb;
    colour = texColour;
}
