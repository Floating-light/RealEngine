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
#include <functional>

#include "Shaders/Shader.h"
#include "utility/TextRender.h"
#include "utility/stb_image.h"
#include "utility/SceneObject.h"
#include "utility/Camera.h"
//#define nullptr 100

float currentVisible = 0.2;
Shader* shad;
TextRender* textRend;

std::vector<KeyEventCallback> KeyEvents;
std::vector<KeyProcess> KeyProc;
std::vector<MouseCursorMoveCallBack> MouseCursorMove;
std::vector<MouseScrollCallBack> MouseScroll;

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
	for (auto& item : KeyEvents)
	{
		item(key, action);
	}
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
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	for (auto& item : MouseCursorMove)
	{
		item(xpos, ypos);
	}
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	for (auto& item : MouseScroll)
	{
		item(xoffset, yoffset);
	}
}

void processInput(GLFWwindow* window, const Shader& shader, float DeltaTime)
{
	for(auto& item: KeyProc)
	{
		item(window, DeltaTime);
	}

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



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

void SetupMultipleShader(Shader& shader, const RCamera& camera, glm::vec3 pointLightPositions[])
{
	shader.setVec3("eyePos", camera.Location);

	shader.setInteger("emissionV", 2);

	// material 
	shader.setInteger("material.diffuse", 0);
	shader.setInteger("material.specular", 1);
	shader.setFloat("material.shininess", 128.0f);

	// direction light
	shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	shader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	// spotlight
	shader.setVec3("spotLight.spotPosition", camera.Location);
	shader.setVec3("spotLight.spotDirection", camera.Front);
	shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("spotLight.outCutOff", glm::cos(glm::radians(17.5f)));
	shader.setVec3("spotLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	// point light
	shader.setVec3("pointlight[0].position", pointLightPositions[0]);
	/*shader.setVec3("light.ambient", lightColor * glm::vec3(0.5f)* glm::vec3(0.2f));
	shader.setVec3("light.diffuse", lightColor * glm::vec3(0.5f));*/
	shader.setVec3("pointlight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointlight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointlight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setFloat("pointlight[0].constant", 1.0f);
	shader.setFloat("pointlight[0].linear", 0.09f);
	shader.setFloat("pointlight[0].quadratic", 0.032f);

	shader.setVec3( "pointlight[1].position", pointLightPositions[1]);
	shader.setVec3( "pointlight[1].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3( "pointlight[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3( "pointlight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setFloat("pointlight[1].constant", 1.0f);
	shader.setFloat("pointlight[1].linear", 0.09f);
	shader.setFloat("pointlight[1].quadratic", 0.032f);

	shader.setVec3( "pointlight[2].position", pointLightPositions[2]);
	shader.setVec3( "pointlight[2].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3( "pointlight[2].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3( "pointlight[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setFloat("pointlight[2].constant", 1.0f);
	shader.setFloat("pointlight[2].linear", 0.09f);
	shader.setFloat("pointlight[2].quadratic", 0.032f);

	shader.setVec3(" pointlight[3].position", pointLightPositions[3]);
	shader.setVec3(" pointlight[3].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3(" pointlight[3].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3(" pointlight[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setFloat("pointlight[3].constant", 1.0f);
	shader.setFloat("pointlight[3].linear", 0.09f);
	shader.setFloat("pointlight[3].quadratic", 0.032f);
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

	// �������ָ��
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ����ָ��window ��framebuffer size ��callback.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glViewport(0, 0, 800, 600);

	// init shader
	Shader cubeShader("lightingMap.vert", "multipleLighting.frag");
	shad = &cubeShader;

	Shader lightShader("lightingCube.vert", "light.frag");

	// creat cube 
	RSceneObject Obj = RSceneObject::Create3DCube();
	RSceneObject::CreateVAO(Obj);
	//KeyEvents.push_back(std::bind(&RSceneObject::InputEvent, &Obj, std::placeholders::_1, std::placeholders::_2));

	RSceneObject lightObj = RSceneObject::Create3DCube();
	RSceneObject::CreateVAO(lightObj);
	lightObj.Translate = glm::vec3(1.2f, 1.0f, 2.0f);
	lightObj.Scale = glm::vec3(0.3f, 0.3f, 0.3f);

	// camera
	RCamera Camera;
	KeyEvents.push_back(std::bind(&RCamera::InputEvent, &Camera, std::placeholders::_1, std::placeholders::_2));
	KeyProc.push_back(std::bind(&RCamera::InputProcess, &Camera, std::placeholders::_1, std::placeholders::_2));
	MouseCursorMove.push_back(std::bind(&RCamera::EventMouseMove, &Camera, std::placeholders::_1, std::placeholders::_2));
	MouseScroll.push_back(std::bind(&RCamera::EventMouseScroll, &Camera, std::placeholders::_1, std::placeholders::_2));

	// load texture
	unsigned int TextureID0, TextureID1, TextureID2;
	ReadImageToTexture("./resources/textures/container2.png", GL_TEXTURE0, TextureID0, GL_RGBA);
	//ReadImageToTexture("./resources/textures/container2_specular.png", GL_TEXTURE1, TextureID1, GL_RGBA);
	ReadImageToTexture("./resources/textures/container2_specular_colored.png", GL_TEXTURE1, TextureID1, GL_RGBA);
	ReadImageToTexture("./resources/textures/matrix.jpg", GL_TEXTURE2, TextureID2, GL_RGB);

	// init debug text render
	TextRender textR("resources/fonts/arial.ttf", glm::vec3(0.5f, 0.8f, 0.2f));
	textRend = &textR;

	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};



	float deltaTime = 0.0f, lastFrameTime = 0.0f;
	// glfwGetTime()
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;
		//lightObj.Translate = glm::vec3(sin(glfwGetTime()), 1.0f, cos(glfwGetTime()));
		// ����ָ��window ������
		processInput(window, cubeShader, deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ��������������Ԫ
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureID1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, TextureID2);

		cubeShader.use();
		//cubeShader.setVec3("eyePos", Camera.Location);

		////glm::vec3 lightColor(sin(glfwGetTime() * 2.0f), sin(glfwGetTime() * 0.7f), sin(glfwGetTime() * 1.3f));
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

		//cubeShader.setVec3("light.position", lightObj.Translate);
		//cubeShader.setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		///*cubeShader.setVec3("light.ambient", lightColor * glm::vec3(0.5f)* glm::vec3(0.2f));
		//cubeShader.setVec3("light.diffuse", lightColor * glm::vec3(0.5f));*/
		//cubeShader.setVec3("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		//cubeShader.setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		//cubeShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//cubeShader.setFloat("light.constant", 1.0f);
		//cubeShader.setFloat("light.linear", 0.09f);
		//cubeShader.setFloat("light.quadratic", 0.032f);

		//cubeShader.setVec3("light.spotPosition", Camera.Location);
		//cubeShader.setVec3("light.spotDirection", Camera.Front);
		//cubeShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		//cubeShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

		//cubeShader.setInteger("material.diffuse", 0);
		//cubeShader.setInteger("material.specular", 1);
		//cubeShader.setFloat("material.shininess", 128.0f);
		//cubeShader.setInteger("emissionV", 2);

		SetupMultipleShader(cubeShader, Camera, pointLightPositions);


		//Camera.Location = Vector3D(sin(glfwGetTime()) * Camera.RotationRadius, 0.0f, cos(glfwGetTime()) * Camera.RotationRadius);
		glUniformMatrix4fv(glGetUniformLocation(cubeShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(Obj.GetModelTrasform()));
		glUniformMatrix4fv(glGetUniformLocation(cubeShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(Camera.GetViewTransform()));
		glUniformMatrix4fv(glGetUniformLocation(cubeShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(Camera.GetPerspective()));
		/*glUniform1i(glGetUniformLocation(cubeShader.ID, "Texture1"), 0);
		glUniform1i(glGetUniformLocation(cubeShader.ID, "Texture2"), 1);*/
		


		glBindVertexArray(Obj.VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		for (int i = 0; i < 10; ++i)
		{
			glm::mat4 M_model(1.0f);
			M_model = glm::translate(M_model, cubePositions[i]);
			float angle = 20.0f * i;
			M_model = glm::rotate(M_model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(cubeShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(M_model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// ��Դcube
		lightShader.use();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightObj.GetModelTrasform()));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(Camera.GetViewTransform()));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(Camera.GetPerspective()));
		lightShader.setVec3("lightColor", lightColor);
		glBindVertexArray(lightObj.VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		for (int i = 0; i < 4; ++i)
		{
			lightObj.Translate = pointLightPositions[i];
			lightObj.Scale = glm::vec3(0.3f, 0.3f, 0.3f);
			glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightObj.GetModelTrasform()));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);


		// draw debug text
		textR.DrawOnScreenDebugMessage(deltaTime);

		glfwPollEvents(); // keyboard input, mouse movement events

		glfwSwapBuffers(window); // ����buffer, ��ʾ���µ���Ⱦ���
	}

	glfwTerminate();

	return 0;
}
/*******************************************************************************************************/
// ��һ��������, �� �� vertex shader ����������, ��������в�ֵ�����뵽 fargment shader