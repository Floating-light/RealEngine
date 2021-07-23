#pragma once
#include <string>
#include <map>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "../Shaders/Shader.h"


struct Character
{
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

struct ScreenMessageString
{
	std::string Message;
	glm::vec3 Color;
	float TimeToDisplay;
	float CurrentDisplayTime;
	float TextScale;
	ScreenMessageString(const std::string& Messag, glm::vec3 DisplayColor, float DisplayTime, float Scale) :
		Message(Messag),
		Color(DisplayColor),
		TimeToDisplay(DisplayTime),
		CurrentDisplayTime(0.0f),
		TextScale(Scale)
	{

	}
};

class TextRender
{
public:
	TextRender(const std::string& fontPath, const glm::vec3& c);
	
	~TextRender();

	void Initialize(const std::string& fontPath, const glm::vec3& c);

	/*
	* @param text The content to display 
	* @param Color The Color of the text
	* @param x , y The begin position of the text to print, left bottom is (0,0)
	* @param scale 
	*/
	void RenderText(const std::string& text, glm::vec3 Color, float x, float y, float scale);

	void DrawOnScreenDebugMessage(float DeltaTime);

	void SetFonts(const std::string& fontPath);

	void AddScreenDebugMessage(const std::string& Message, glm::vec3 DisplayColor, float DisplayTime, float Scale = 1.0f);
private:
	void InitDraw();

private:

	float FontSizeHeight;

	std::map<char, Character> Characters;

	Shader* MyTextShader;

	unsigned int VAO;

	unsigned int VBO;

	std::vector<ScreenMessageString> DebugMessages;
};