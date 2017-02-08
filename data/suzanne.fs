#version 330

out vec4 colour;
uniform float time;

smooth in float dist;

float map(float value, float inMin, float inMax, float outMin, float outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

void main()
{
	if(dot(gl_PointCoord-0.5,gl_PointCoord-0.5)>0.25)
		discard;
	else if (dot(gl_PointCoord-0.5,gl_PointCoord-0.5)>0.15) 
		colour = vec4(sin(time), cos(time), 0.2, 1.0);
	else
		colour = vec4(cos(time), sin(time), 0.2, 1.0);
}
