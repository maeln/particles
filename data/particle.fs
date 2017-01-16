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
	else {
		float g = (dot(gl_PointCoord-0.5,gl_PointCoord-0.5) > 0.22 ? 0.6 : map(dot(gl_PointCoord-0.5,gl_PointCoord-0.5), 0.0, 0.21, 0.0, 0.6));
		colour = vec4(g, 0.2, 0.6, 0.5+(1+sin(time*2))/6.0);
	}
}
