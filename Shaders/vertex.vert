#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;
uniform float offset;
uniform mat4 transform;
out vec3 vertexColor;
out vec2 texCoord;
void main()
{
	gl_Position = transform * vec4(aPos, 1.0);
	vertexColor = color;
	texCoord = uv;
}