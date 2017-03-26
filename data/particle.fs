#version 330

out vec4 colour;
uniform float time;

smooth in float dist;
in vec3 vcolour;

/*
float map(float value, float inMin, float inMax, float outMin, float outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}
*/

void main()
{
	if(dot(gl_PointCoord-0.5,gl_PointCoord-0.5)>0.25)
		discard;
	else
		colour = vec4(vcolour, 1.0-4*dot(gl_PointCoord-0.5,gl_PointCoord-0.5));
}
