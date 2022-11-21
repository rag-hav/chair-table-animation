#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

uniform mat4 mpv;
uniform vec3 vertexColor;
out vec3 fragmentColor;


void main()
{
	gl_Position = mpv * position;
    fragmentColor = vertexColor;
}
