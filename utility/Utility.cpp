#include "Utility.h"

#include <iostream>

#include "stb_image.h"
#include "glad/glad.h"

unsigned int Utility::TextureFromFile(const std::string& texName, const std::string& dir)
{
	unsigned int textureID;
	std::string filePath = dir + "/" + texName;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	//float borderColor[] = { 1.0f, 0.0f, 0.0f, 0.5f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); // 如果上面用了GL_CLAMP_TO_BORDER, 则要这样指定边界颜色
	// 设置边界
	/*
	*	GL_REPEAT
	*	GL_MIRRORED_REPEAT
	*	GL_CLAMP_TO_EDGE
	*	GL_CLAMP_TO_BORDER
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// texture 采样
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true); // image y 方向翻转.
	unsigned char* data = stbi_load(filePath.data(), &width, &height, &channels, 0);
	if (data)
	{
		GLenum sourceFormat;
		if (channels == 1)
		{
			sourceFormat = GL_RED;
		}
		else if (channels == 3)
		{
			sourceFormat = GL_RGB;
		}
		else if (channels == 4)
		{
			sourceFormat = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, sourceFormat, width, height, 0, sourceFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture : " << filePath << std::endl;
	}
	stbi_image_free(data);

	return textureID;
}
