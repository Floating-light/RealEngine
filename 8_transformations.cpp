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
float rotationDegree;
glm::vec3 translate(0.0f, 0.0f, 0.0f);
glm::vec3 myScale(1.0f, 1.0f, 1.0f);
//struct Character
//{
//	unsigned int TextureID;
//	glm::ivec2 size;
//	glm::ivec2 Bearing;
//	unsigned int Advance;
//};

namespace ColorType
{
	glm::vec3 RED     (1.0f, 0.0f, 0.0f);
	glm::vec3 GREEN   (0.0f, 1.0f, 0.0f);
	glm::vec3 BLUE    (0.0f, 0.0f, 1.0f);
	glm::vec3 YELLOW  (1.0f, 1.0f, 0.0f);
	glm::vec3 PINK    (1.0f, 0.0f, 1.0f);
	glm::vec3 PURPLE  (0.6f, 0.0f, 1.0f);
};

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
		textRend->AddScreenDebugMessage("Debug Key pressed------>>" + std::to_string(glfwGetTime()), ColorType::PURPLE, 2.0f);
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		rotationDegree += 5;
		textRend->AddScreenDebugMessage("Rotate angle : " + std::to_string(rotationDegree), ColorType::PINK, 2);
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		rotationDegree -= 5;
		textRend->AddScreenDebugMessage("Rotate angle : " + std::to_string(rotationDegree), ColorType::PINK, 2);

	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		translate.x -= 0.1;
		textRend->AddScreenDebugMessage("Translate x : " + std::to_string(translate.x), ColorType::YELLOW, 2);

	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		translate.x += 0.1;
		textRend->AddScreenDebugMessage("Translate x : " + std::to_string(translate.x), ColorType::YELLOW, 2);

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
	glViewport(0, 0, 800, 600);

	Shader shader("vertex.vert", "fragment.frag");
	shad = &shader;
	shader.use();
	glUniform1f(glGetUniformLocation(shader.ID, "visible"), currentVisible);
	// vertex array object, vertex buffer object, element buffer object
	unsigned int VAO, VBO, EBO;
	ConfigVertexArrayObejcts(VAO, VBO, EBO);

	unsigned int TextureID0, TextureID1;
	ReadImageToTexture("./resources/textures/container.jpg", GL_TEXTURE0, TextureID0, GL_RGB);
	ReadImageToTexture("./resources/textures/awesomeface.png", GL_TEXTURE1, TextureID1, GL_RGBA);

	TextRender textR("resources/fonts/arial.ttf", glm::vec3(0.5f, 0.8f, 0.2f));
	textRend = &textR;
	
	std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
	textR.AddScreenDebugMessage("AHHHHHHHHHHHH1_5", { 25.0f, 25.0f, 1.0f }, 5);
	textR.AddScreenDebugMessage("222222222222_10", { 25.0f, 25.0f, 1.0f }, 10);
	textR.AddScreenDebugMessage("333333333333_15", { 25.0f, 25.0f, 1.0f }, 15);

	// glm 
	//glm::vec4 vec(1.0f, 0.f, 0.f, 1.0f);
	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	//vec = trans * vec;
	//std::cout << vec.x << ", "<<  vec.y << ", "<< vec.z << std::endl;

	glm::mat4 trans(1.0);
	trans = glm::rotate(trans, glm::radians(rotationDegree), glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

	
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// 处理指定window 的输入
		shader.use();
		processInput(window, shader);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 绑定纹理到纹理单元
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureID1);

		// 绑定纹理单元到shader
		glUniform1i(glGetUniformLocation(shader.ID, "baseColorTexture"), 0);
		shader.setInteger("anotherTex", 1);

		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, translate);
		trans = glm::rotate(trans, glm::radians(rotationDegree + static_cast<float>(glfwGetTime())), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f) * sinf(glfwGetTime()));
		// trans = trans * translate * rotate * scale
		// trans * point 注意顺序

		/*
		* #param transpose 转置
		*/
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		 
		// 绑定要绘制的vertex
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		glm::mat4 trs2(1.0f);
		trs2 = glm::translate(trs2, glm::vec3(-0.5f, 0.5f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(trs2));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		const auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - tp);
		tp = std::chrono::steady_clock::now();
		textR.DrawOnScreenDebugMessage(dur.count());

		shader.use();// 输入需要改变shader中的uniform
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