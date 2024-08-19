#version 330 core

in vec2 TexCoord;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

out vec4 outColor;
void main()
{
	outColor = mix(texture(Texture1, TexCoord), texture(Texture2,TexCoord), 0.5);
}