#version 330

smooth in vec4 interpol;

out vec4 color;

void main()
{
	color = interpol;
	//color = vec4(1.0, 0.0, 0.0, 1.0);
}
