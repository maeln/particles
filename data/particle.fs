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
		colour = vec4(151.0/255.0, 189.0/255.0, 235.0/255.0, 1.0);
	else
		colour = vec4(80.0/255.0, 119.0/255.0, 164.0/255.0, 1.0);
}
