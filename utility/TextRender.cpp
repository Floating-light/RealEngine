#include "TextRender.h"

#include "glad/glad.h"
#include "ft2build.h"
#include FT_FREETYPE_H

#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"


TextRender::TextRender(const std::string& fontPath, const glm::vec3& c) : FontSizeHeight(32)
{
	Initialize(fontPath, c);
}

TextRender::~TextRender()
{
	for (auto item : Characters)
	{
		glDeleteTextures(1, &item.second.TextureID);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	if (MyTextShader)
	{
		delete MyTextShader;
	}
}

void TextRender::Initialize(const std::string& fontPath, const glm::vec3& c)
{
	SetFonts(fontPath);
	InitDraw();
}

void TextRender::AddScreenDebugMessage(const std::string& Message, glm::vec3 DisplayColor, float DisplayTime, float Scale)
{
	DebugMessages.emplace_back(Message, DisplayColor, DisplayTime, Scale);
}

void TextRender::DrawOnScreenDebugMessage(float DeltaTime)
{
	const float FrameSeconds = DeltaTime;
	float BeginX = 0, BeginY= 0;
	for (int i = DebugMessages.size() - 1; i >= 0; --i)
	{
		ScreenMessageString& Message = DebugMessages[i];
		BeginY += FontSizeHeight * Message.TextScale;
		RenderText(Message.Message, Message.Color, BeginX, BeginY, Message.TextScale);
		Message.CurrentDisplayTime += FrameSeconds;
		if (Message.CurrentDisplayTime > Message.TimeToDisplay)
		{
			DebugMessages.erase(DebugMessages.begin() + i);
		}
	}
	RenderText("Frame rate : " + std::to_string(1.0f / FrameSeconds), glm::vec3(0.0f, 1.0f, 0.0f),600, 500,0.5f);
	RenderText("Frame time : " + std::to_string(DeltaTime * 1000.0f) + " ms" , glm::vec3(0.0f, 1.0f, 0.0f), 600, 440, 0.5f);
}


void TextRender::RenderText(const std::string& text, glm::vec3 Color, float x, float y, float scale)
{
	MyTextShader->use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform3f(glGetUniformLocation(MyTextShader->ID, "textColor"), Color.x, Color.y, Color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	for (auto c = text.cbegin(); c != text.end(); ++c)
	{
		Character ch = Characters[*c];
		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,    ypos + h, 0.0f, 0.0f},
			{ xpos,    ypos,     0.0f, 1.0f},
			{ xpos + w, ypos,    1.0f, 1.0f},

			{ xpos,    ypos + h, 0.0f, 0.0f},
			{ xpos + w, ypos,    1.0f, 1.0f},
			{ xpos + w, ypos + h, 1.0f, 0.0f}
		};

		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// advance cursors for next glyph (advance is 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//"resources/fonts/arial.ttf"
void TextRender::SetFonts(const std::string& fontPath)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) // initialize the freetype library
	{
		std::cout << "ERROR::FREERTPE: Could not init FreeType Library" << std::endl;
	}
	FT_Face face; // hosts a collection of glyphs
	if (FT_New_Face(ft, fontPath.data(), 0, &face))// load the fonts as a 'face'
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	// set the pixel font size to extract,
	// if width == 0, dynamically calculate the width by given height
	FT_Set_Pixel_Sizes(face, 0, FontSizeHeight);

	// set active glyph to 'X'
	// FT_LOAD_RENDER create a 8-bit grayscale bitmap image,
	// can access via face->glyph->bitmap
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
	}

	// opengl 要求textures 4byte对齐, 通常这不成问题, 一个像素RGBA刚好4个bytes.
	// 但用一个byte来表示一个像素时, 要强制设置对齐为1 byte.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// 8-bit -- 1-byte 灰度图
		// 通过将texture color's red component(color vector 中的第一个byte) 视作灰度图像中的一个byte.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		// set texture options 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use 
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRender::InitDraw()
{
	// for render text
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 动态改变内存中的内容
	// 两个三角形6个顶点, 组成一个四边形
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	MyTextShader = new Shader("glyphs.vert", "glyphs.frag");
	MyTextShader->use();
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	glUniformMatrix4fv(glGetUniformLocation(MyTextShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}