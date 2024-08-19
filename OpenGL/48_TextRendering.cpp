#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>

#include "Shaders/Shader.h"
#include "utility/TextRender.h"
#include "utility/stb_image.h"

//#define nullptr 100

float currentVisible = 0.2;
Shader* shad;
TextRender* textRend;
//struct Character
//{
//	unsigned int TextureID;
//	glm::ivec2 size;
//	glm::ivec2 Bearing;
//	unsigned int Advance;
//};

typedef glm::vec3 ColorType;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int cancode, int action, int mods)
{
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		shad->setFloat("visible", (currentVisible += 0.1));
		textRend->AddScreenDebugMessage("Set currentVisible as : " + std::to_string(currentVisible), glm::vec3{ 0.0f,0.0f, 0.8f }, 1.0f);

	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		shad->setFloat("visible", (currentVisible -= 0.1));
		textRend->AddScreenDebugMessage("Set currentVisible as : " + std::to_string(currentVisible), glm::vec3{ 0.0f,0.0f, 0.8f }, 1.0f);

	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		textRend->AddScreenDebugMessage("Debug Key pressed", ColorType{ 0.6f, 0.0f, 0.0f }, 2.0f);
	}
}

void processInput(GLFWwindow* window, const Shader& shader)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		for (int i = 0; i < 1000; ++i)
		{
			std::cout << "hahahahahahha" << std::endl;
		}
		//shader.setFloat("visible", (currentVisible += 0.1));
		//textRend->AddScreenDebugMessage("Set currentVisible as : " + std::to_string(currentVisible), glm::vec3{ 0.0f,0.0f, 0.8f }, 1.0f);
	}
	//else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	//{
	//	shader.setFloat("visible", (currentVisible -= 0.1));
	//	textRend->AddScreenDebugMessage("Set currentVisible as : " + std::to_string(currentVisible), glm::vec3{ 0.0f,0.0f, 0.8f }, 1.0f);

	//}
}

enum class InputVertexID
{
	TriangleCenter,
	TriangleLeft,
	TriangleRight,
	Rectangle
};

// Get The vertices to rendering
std::vector<float>& GetVertices(const InputVertexID Type = InputVertexID::TriangleCenter)
{
	// 单个三角形
	static std::vector<float > VertTriangleCenter = {
			 -0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
			  0.5f, -0.5f, 0.0f,0.0f, 1.0f, 0.0f,
			  0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, };

	static std::vector<float > VertTriangleLeft = {
			 -1.f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
			 -0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
			  0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, };

	static std::vector<float > VertTriangleRight = {
		  1.f, -0.5f, 0.0f,
		  0.5f, 0.5f, 0.0f,
		  0.0f, -0.5f, 0.0f };

	// 正方形                                     // cooridinate   colors            texture coords
	static std::vector<float > VertRectangle = { 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
												 0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
												-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
												-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	if (Type == InputVertexID::TriangleCenter)
	{
		return VertTriangleCenter;
	}
	else if (Type == InputVertexID::TriangleLeft)
	{
		return VertTriangleLeft;
	}
	else if (Type == InputVertexID::TriangleRight)
	{
		return VertTriangleRight;
	}
	else if (Type == InputVertexID::Rectangle)
	{
		return VertRectangle;
	}
	return VertTriangleCenter;
}

std::vector<unsigned int>& GetIndices()
{
	static std::vector<unsigned int> indices = { 0,1,3,
												1,2,3 };
	return indices;
}

void ReadImageToTexture(const std::string& imagePath, GLenum textureUnit, GLenum& textureID, GLenum sourceFormat)
{
	glGenTextures(1, &textureID);

	glActiveTexture(textureUnit);

	glBindTexture(GL_TEXTURE_2D, textureID);
	float borderColor[] = { 1.0f, 0.0f, 0.0f, 0.5f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); // 如果上面用了GL_CLAMP_TO_BORDER, 则要这样指定边界颜色
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



	int width, height, channels;
	stbi_set_flip_vertically_on_load(true); // image y 方向翻转.
	unsigned char* data = stbi_load(imagePath.data(), &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, sourceFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture : " << imagePath << std::endl;
	}
	stbi_image_free(data);
}
// 配置Texture unit
unsigned int ConfigTexture()
{
	unsigned int textureID;
	/*
	* param1 生成多少个texture
	* param2 一个ID指针, 生成多少个texture就会有多少个元素, 该指针指向第一个ID, 这里只有一个, 所以直接给个局部变量的地址也可以
	*/
	glGenTextures(1, &textureID);

	// GL_TEXTURE0 ~~~ GL_TEXTURE15 都是连续的
	glActiveTexture(GL_TEXTURE0);
	// 绑定创建的texture到GL_TEXTURE_2D, 后面所有对GL_TEXTURE_2D的操作将都是对textureID所指向的Texture.
	glBindTexture(GL_TEXTURE_2D, textureID);

	// 当纹理坐标u, v超出[0，1]时的处理方式.
	// GL_REPEAT 重复
	// GL_MIRRORED_REPEAT 镜像重复
	// GL_CLAMP_TO_EDGE 超出部分用边缘值填充
	// GL_CLAMP_TO_BORDER 超出部分用自己设置的值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); // 如果上面用了GL_CLAMP_TO_BORDER, 则要这样指定边界颜色

	// 纹理放大和纹理minification(supersampling), 图片太大
	// GL_TEXTURE_MIN_FILTER texture 范围查询, 模型过小, 纹理分辨率过大, 用mipmap + 3线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 纹理放大, 双线性插值

	int width, height, nrChannels;
	unsigned char* data = stbi_load("./model/spot_texture.png", &width, &height, &nrChannels, 0);

	if (data != nullptr)
	{
		/*
		* param1 指定这次操作的目标, GL_TEXTURE_2D, GL_TEXTURE_1D, GL_TEXTURE_3D
		* param2 当前读入图片mipmap 层级,如果手动读入不同层级则增加这个值
		* param3 texture 保存的图像格式, 可从原图像转过来
		* param4 param5 texture 的长和宽
		* param6 always 0, some legacy stuff.
		* param7 原图片数据的格式
		* param8 原图片的数据类型
		* param9 原图片数据
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); // 自动生成Mipmap
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	return textureID;
}
// 先配置好所有要用的VBO, attributes pointers 为VAO, 然后保存这些VAO, 后面再用. 
void ConfigVertexArrayObejcts(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO)
{
	//float* vertices = GetVertices(); // 无法返回完整的数组, 会被当成一个数.
	auto& vvector = GetVertices(InputVertexID::Rectangle);
	auto& indices = GetIndices();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vvector.size() * sizeof(float), vvector.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, vvector.size() * sizeof(float) , vvector.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// vertex coor
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coor, u, v
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// 在unbind VAO之前都不能unbind EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // error VAO also remenber the unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Draw use:glDrawArrays(GL_TRIANGLES, 0, 6); 
void ConfigVertexArrayObejct_Gen(unsigned int& VAO, unsigned int& VBO, InputVertexID type)
{
	auto& data = GetVertices(type);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);



	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, vvector.size() * sizeof(float) , vvector.data(), GL_STATIC_DRAW);

	// vertex cooridinate
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coor, u,v
	/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);*/


	// 在unbind VAO之前都不能unbind EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // error VAO also remenber the unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ConfigFonts(std::map<char, Character>& Characters, unsigned int &textVAO, unsigned int &textVBO)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) // initialize the freetype library
	{
		std::cout << "ERROR::FREERTPE: Could not init FreeType Library" << std::endl;
	}
	FT_Face face; // hosts a collection of glyphs
	if (FT_New_Face(ft, "resources/fonts/arial.ttf", 0, &face))// load the fonts as a 'face'
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	// set the pixel font size to extract,
	// if width == 0, dynamically calculate the width by given height
	FT_Set_Pixel_Sizes(face, 0, 48);

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

	// for render text
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);

	glBindVertexArray(textVAO);

	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	// 动态改变内存中的内容
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RenderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color, std::map<char, Character>& Characters, const unsigned int &VAO, const unsigned int & VBO)
{
	// active corresponding render state
	s.use();
	glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	
	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); ++c)
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

int main()
{
	// Init glfw
	glfwInit();
	// Specify gl version 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Gl api version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a display window 
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); // 
		return -1;
	}
	// make a opengl context from previous created window
	glfwMakeContextCurrent(window);

	// load all opengl function by use the function provided by glfw via glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 设置指定window 的framebuffer size 的callback.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	//glViewport(0, 0, 800, 600);

	Shader shader("vertex.vert", "fragment.frag");
	shad = &shader;
	shader.use();
	glUniform1f(glGetUniformLocation(shader.ID, "visible"), currentVisible);

	/*Shader textShader("glyphs.vert", "glyphs.frag");
	textShader.use();
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));*/

	// vertex array object, vertex buffer object, element buffer object
	unsigned int VAO, VBO, EBO;
	// Exercises 2
	ConfigVertexArrayObejcts(VAO, VBO, EBO);


	/*unsigned int VAO2, VBO2;
	ConfigVertexArrayObejct_Gen(VAO2, VBO2, InputVertexID::TriangleRight);*/


	// wireframe rendering
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	unsigned int TextureID0, TextureID1;
	ReadImageToTexture("./resources/textures/container.jpg", GL_TEXTURE0, TextureID0, GL_RGB);
	//ReadImageToTexture("./resources/textures/wood.png", GL_TEXTURE1, TextureID1, GL_RGB);
	ReadImageToTexture("./resources/textures/awesomeface.png", GL_TEXTURE1, TextureID1, GL_RGBA);

	//std::map<char, Character> Characters;
	//unsigned int textVAO, textVBO;
	//ConfigFonts(Characters, textVAO, textVBO);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	TextRender textR("resources/fonts/arial.ttf", glm::vec3(0.5f, 0.8f, 0.2f));
	textRend = &textR;
	
	std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
	textR.AddScreenDebugMessage("AHHHHHHHHHHHH1_5", { 25.0f, 25.0f, 1.0f }, 5);
	textR.AddScreenDebugMessage("222222222222_10", { 25.0f, 25.0f, 1.0f }, 10);
	textR.AddScreenDebugMessage("333333333333_15", { 25.0f, 25.0f, 1.0f }, 15);
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// 处理指定window 的输入
		shader.use();
		processInput(window, shader);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glUseProgram(shaderPrograme);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureID1);

		glUniform1i(glGetUniformLocation(shader.ID, "baseColorTexture"), 0);
		shader.setInteger("anotherTex", 1);
		//float timeValue = glfwGetTime();
		//float greenValue = abs(sin(timeValue * 5) / 2.0f) + 0.5f;
		//// must use this shader first
		//shader.setVec4("colorChanged", 0.0f, greenValue, 0.0f, 0.0f);
		//shader.setFloat("offset", timeValue);
		glBindVertexArray(VAO);

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureID1);*/

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		

		/*shader.setVec4("colorChanged", 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);*/
		const auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - tp);
		tp = std::chrono::steady_clock::now();
		textR.DrawOnScreenDebugMessage(dur.count());

		shader.use();

		glfwPollEvents(); // keyboard input, mouse movement events
		glfwSwapBuffers(window); // 交换buffer, 显示最新的渲染结果
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}


/** *****************************************************************************************************/