#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model; // the model transform of scene object
uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = lightSpaceMatrix * model * vec4(aPos, 1);
}