#version 330

layout(location = 0) in vec3 position;

uniform mat4 isplane;
uniform mat4 projection;
uniform mat4 view;
// uniform mat4 model;

smooth out vec4 interpol;

void main() {
    vec4 npos = view * vec4(position, 1.0);
    gl_Position = projection * npos;
    interpol = vec4(clamp(vec4(position, 1.0), 0.0, 1.0));
}
