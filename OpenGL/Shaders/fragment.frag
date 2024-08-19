#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;

uniform sampler2D baseColorTexture; // 从GL_TEXTURE_2D 绑定的image 读取
uniform sampler2D anotherTex;
uniform float visible;
void main()
{
    // second parameter is 20%
    FragColor = mix(texture(baseColorTexture, texCoord), texture(anotherTex, vec2(texCoord.x, texCoord.y)), visible);
}