#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view ;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 fragCoord;
out vec3 Normal;

void main()
{
	TexCoord = uv;
	gl_Position = projection * view * model * vec4(aPos, 1);
	fragCoord = vec3(model * vec4(aPos, 1));// 这个像素的世界坐标
	Normal = mat3(transpose(inverse(model))) * normal;
}