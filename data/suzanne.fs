#version 330

out vec4 colour;
uniform float time;

smooth in float dist;

void main()
{
	if(dot(gl_PointCoord-0.5,gl_PointCoord-0.5)>0.25)
		discard;
	else
		colour = vec4(sin(dist), 0, cos(dist), 1.0-4*dot(gl_PointCoord-0.5,gl_PointCoord-0.5));
}
