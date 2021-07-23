#version 330 core

in vec2 TexCoord;
uniform vec3 InColor;
out vec4 outColor;

void main()
{
	outColor = vec4(InColor, 1.0);
}