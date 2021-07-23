#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view ;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec2 TexCoord;
out vec3 fragCoord;
out vec3 Normal;
out VS_OUT{
	vec3 FragPos; // the world position of this fragment.
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace; // the light space world cooridinate of this fragment
} vs_out;

void main()
{
	vs_out.TexCoords = uv;
	vs_out.FragPos = vec3(model * vec4(aPos, 1));// 这个像素的世界坐标
	vs_out.Normal = mat3(transpose(inverse(model))) * normal; //
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos,1.0);
	gl_Position = projection * view * model * vec4(aPos, 1);
}