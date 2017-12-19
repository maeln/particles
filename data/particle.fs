#version 330

out vec4 colour;
uniform float time;

smooth in float dist;
in float pttl;

void main()
{
	if(dot(gl_PointCoord-0.5,gl_PointCoord-0.5)>0.25)
		discard;
	else
		colour = vec4(mix(1.0, 0.0, 1.0-(2.5-pttl)), 0.0, mix(1.0, 0.0, 2.5-pttl), 1.0);
}
