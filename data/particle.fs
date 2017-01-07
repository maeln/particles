#version 330

out vec4 colour;
uniform float time;

float map(float value, float inMin, float inMax, float outMin, float outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

void main()
{
//	colour = vec4(pos.x, pos.y, 1.0, 1.0);
	if(dot(gl_PointCoord-0.5,gl_PointCoord-0.5)>0.25)
		discard;
	else {
		float g = (dot(gl_PointCoord-0.5,gl_PointCoord-0.5) > 0.22 ? 0.6 : map(dot(gl_PointCoord-0.5,gl_PointCoord-0.5), 0.0, 0.21, 0.0, 0.6));
		colour = vec4(g, g*sin(time)*sin(time)*cos(time), sin(time), 1.0);
	}
}
