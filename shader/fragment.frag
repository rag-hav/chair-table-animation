#version 330

out vec3 outputColor;
in vec3 fragmentColor;

void main()
{
	outputColor = fragmentColor;
}
