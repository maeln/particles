#include <string>
#include <glm/vec4.hpp>

struct Point
{
	glm::vec4 pos;
	glm::vec4 dir;
	double ttl;
};

const std::string vertex_shader(
"#version 330\n"
"layout(location = 0) in vec4 position;\n"
//"smooth out vec2 pos;\n"
"void main()\n"
"{\n"
"	gl_Position = position;\n"
//"	pos = position.xy;\n"
"}\n"
);

const std::string fragment_shader(
"#version 330\n"
//"smooth in vec2 pos;"
"out vec4 colour;\n"
"void main()\n"
"{\n"
//"	colour = vec4(pos.x, pos.y, 1.0, 1.0);\n"
"	colour = vec4(gl_FragCoord.x/600.0, gl_FragCoord.y/600.0, 1.0, 1.0);\n"
"}\n"
);

const std::string geometry_shader(
"#version 330\n"
"layout (points) in;\n"
"layout (triangle_strip, max_vertices = 3) out;\n"
"void main()\n"
"{\n"
"	gl_Position = gl_in[0].gl_Position + vec4(-0.01, 0.0, 0.0, 0.0);\n"
"	EmitVertex();\n"
"	gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.02, 0.0, 0.0);\n"
"	EmitVertex();\n"
"	gl_Position = gl_in[0].gl_Position + vec4(0.01, 0.0, 0.0, 0.0);\n"
"	EmitVertex();\n"
"	EndPrimitive();\n"
"}\n"
);

const float triangle[] = {
	0.5, 0.5, 0.0, 1.0,
	-0.5, -0.5, 0.0, 1.0,
	-0.5, 0.5, 0.0, 1.0,
	0.5, -0.5, 0.0, 1.0
};
