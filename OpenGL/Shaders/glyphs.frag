#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r); 
	// 采样的bitmap值作为最终文字的alpha值, 背景色将是透明的, 文字则不是透明的.
	// 再×上文字颜色, 有点变化
	color = vec4(textColor, 1.0) * sampled;
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 必须设置混合alpha, 否则alpha将不起作用, 全是一些色块
}