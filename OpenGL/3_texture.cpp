#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <vector>
#include <string>

#include "Shaders/Shader.h"

#include "utility/stb_image.h"

float currentVisible = 0.2;
Shader* shad;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int cancode, int action, int mods)
{
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		shad->setFloat("visible", (currentVisible += 0.1));

	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		shad->setFloat("visible", (currentVisible -= 0.1));

	}
}

void processInput(GLFWwindow* window, const Shader& shader)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	//else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	//{
	//	shader.setFloat("visible", (currentVisible += 0.1));
	//}
	//else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	//{
	//	shader.setFloat("visible", (currentVisible -= 0.1));

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
	// ����������
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

	// ������                                     // cooridinate   colors            texture coords
	static std::vector<float > VertRectangle = { 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
											     0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
											    -0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
											    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};
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
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); // �����������GL_CLAMP_TO_BORDER, ��Ҫ����ָ���߽���ɫ
	// ���ñ߽�
	/*
	*	GL_REPEAT
	*	GL_MIRRORED_REPEAT
	*	GL_CLAMP_TO_EDGE
	*	GL_CLAMP_TO_BORDER
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// texture ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



	int width, height, channels;
	stbi_set_flip_vertically_on_load(true); // image y ����ת.
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
// ����Texture unit
unsigned int ConfigTexture()
{
	unsigned int textureID;
	/*
	* param1 ���ɶ��ٸ�texture
	* param2 һ��IDָ��, ���ɶ��ٸ�texture�ͻ��ж��ٸ�Ԫ��, ��ָ��ָ���һ��ID, ����ֻ��һ��, ����ֱ�Ӹ����ֲ������ĵ�ַҲ����
	*/
	glGenTextures(1, &textureID);

	// GL_TEXTURE0 ~~~ GL_TEXTURE15 ����������
	glActiveTexture(GL_TEXTURE0);
	// �󶨴�����texture��GL_TEXTURE_2D, �������ж�GL_TEXTURE_2D�Ĳ��������Ƕ�textureID��ָ���Texture.
	glBindTexture(GL_TEXTURE_2D, textureID);

	// ����������u, v����[0��1]ʱ�Ĵ�����ʽ.
	// GL_REPEAT �ظ�
	// GL_MIRRORED_REPEAT �����ظ�
	// GL_CLAMP_TO_EDGE ���������ñ�Եֵ���
	// GL_CLAMP_TO_BORDER �����������Լ����õ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); // �����������GL_CLAMP_TO_BORDER, ��Ҫ����ָ���߽���ɫ

	// �����Ŵ������minification(supersampling), ͼƬ̫��
	// GL_TEXTURE_MIN_FILTER texture ��Χ��ѯ, ģ�͹�С, �����ֱ��ʹ���, ��mipmap + 3���Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // �����Ŵ�, ˫���Բ�ֵ

	int width, height, nrChannels;
	unsigned char* data = stbi_load("./model/spot_texture.png", &width, &height, &nrChannels, 0);

	if (data != nullptr)
	{
		/*
		* param1 ָ����β�����Ŀ��, GL_TEXTURE_2D, GL_TEXTURE_1D, GL_TEXTURE_3D
		* param2 ��ǰ����ͼƬmipmap �㼶,����ֶ����벻ͬ�㼶���������ֵ
		* param3 texture �����ͼ���ʽ, �ɴ�ԭͼ��ת����
		* param4 param5 texture �ĳ��Ϳ�
		* param6 always 0, some legacy stuff.
		* param7 ԭͼƬ���ݵĸ�ʽ
		* param8 ԭͼƬ����������
		* param9 ԭͼƬ����
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); // �Զ�����Mipmap
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	return textureID;
}
// �����ú�����Ҫ�õ�VBO, attributes pointers ΪVAO, Ȼ�󱣴���ЩVAO, ��������. 
void ConfigVertexArrayObejcts(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO)
{
	//float* vertices = GetVertices(); // �޷���������������, �ᱻ����һ����.
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coor, u, v
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	// ��unbind VAO֮ǰ������unbind EBO
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


	// ��unbind VAO֮ǰ������unbind EBO
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

	// ����ָ��window ��framebuffer size ��callback.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	//glViewport(0, 0, 800, 600);

	Shader shader("vertex.vert", "fragment.frag");
	shad = &shader;
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

	glUniform1f(glGetUniformLocation(shader.ID, "visible"), currentVisible);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// ����ָ��window ������
		processInput(window, shader);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glUseProgram(shaderPrograme);
		shader.use();
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


		glfwPollEvents(); // keyboard input, mouse movement events
		glfwSwapBuffers(window); // ����buffer, ��ʾ���µ���Ⱦ���
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}


/** *****************************************************************************************************/