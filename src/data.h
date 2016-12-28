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
"uniform float time;\n"
"uniform mat4 camera;\n"
//"smooth out vec2 pos;\n"
"void main()\n"
"{\n"
"	vec4 cameraPos = position + vec4(0.0, 0.0, -1.0, 0.0);\n"
"	gl_Position = camera * cameraPos;\n"
"	gl_PointSize = 10.0*dot(camera * cameraPos, position);"
//"	pos = position.xy;\n"
"}\n"
);

const std::string fragment_shader(
"#version 330\n"
//"smooth in vec2 pos;"
"out vec4 colour;\n"
"uniform float time;\n"
"float map(float value, float inMin, float inMax, float outMin, float outMax) {\n"
"  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);\n"
"}\n"
"void main()\n"
"{\n"
//"	colour = vec4(pos.x, pos.y, 1.0, 1.0);\n"
"if(dot(gl_PointCoord-0.5,gl_PointCoord-0.5)>0.25)\n" 
"   discard;\n"
"else {\n"
"	float g = (dot(gl_PointCoord-0.5,gl_PointCoord-0.5) > 0.22 ? 0.6 : map(dot(gl_PointCoord-0.5,gl_PointCoord-0.5), 0.0, 0.21, 0.0, 0.6));"
"	colour = vec4(g, g*sin(time)*sin(time)*cos(time), sin(time), 1.0);\n"
"}\n"
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
