#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 velocity;
layout(location = 2) in float ttl;

uniform float time;
uniform mat4 camera;
uniform mat4 world;
uniform vec4 eye;

out FS {
    out float dist;
    out float ttl;
}
fs;

void main() {
    gl_Position = position;
    fs.ttl = ttl;
    fs.dist = distance(position, eye);
    // fs.tex_coord = tex_coord;
}
