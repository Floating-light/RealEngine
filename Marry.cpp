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
#include "utility/Model.h"
#include "utility/Utility.h"
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
void renderQuad();
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

void processInput(GLFWwindow* window, float DeltaTime)
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



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

void SetupMultipleShader(Shader& shader, const RCamera& camera, glm::vec3 pointLightPositions)
{
	shader.setVec3("eyePos", camera.Location);
	std::cout << "camera.Location : " << camera.Location.x <<  "    "<<  camera.Location.y << "    " << camera.Location.z << std::endl;
	shader.setInteger("emissionV", 2);

	// material 
	shader.setInteger("material.texture_diffuse1", 0);
	shader.setInteger("material.texture_specular1", 1);
	shader.setFloat("material.shininess", 128.0f);

	// point light
	shader.setVec3("pointlight.position", pointLightPositions);
	/*shader.setVec3("light.ambient", lightColor * glm::vec3(0.5f)* glm::vec3(0.2f));
	shader.setVec3("light.diffuse", lightColor * glm::vec3(0.5f));*/
	
	shader.setVec3("pointlight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointlight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setVec3("pointlight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setFloat("pointlight.constant", 1.0f);
	shader.setFloat("pointlight.linear", 0.09f);
	shader.setFloat("pointlight.quadratic", 0.032f);
	
	
	/*shader.setVec3("pointlight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointlight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointlight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setFloat("pointlight[0].constant", 2.0f);
	shader.setFloat("pointlight[0].linear", 0.09f);
	shader.setFloat("pointlight[0].quadratic", 0.032f);

	shader.setVec3( "pointlight[1].position", pointLightPositions[1]);
	shader.setVec3( "pointlight[1].ambient", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setVec3( "pointlight[1].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setVec3( "pointlight[1].specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setFloat("pointlight[1].constant", 2.0f);
	shader.setFloat("pointlight[1].linear", 0.09f);
	shader.setFloat("pointlight[1].quadratic", 0.032f);

	shader.setVec3( "pointlight[2].position", pointLightPositions[2]);
	shader.setVec3( "pointlight[2].ambient", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setVec3( "pointlight[2].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setVec3( "pointlight[2].specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setFloat("pointlight[2].constant", 2.0f);
	shader.setFloat("pointlight[2].linear", 0.09f);
	shader.setFloat("pointlight[2].quadratic", 0.032f);

	shader.setVec3(" pointlight[3].position", pointLightPositions[3]);
	shader.setVec3(" pointlight[3].ambient", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setVec3(" pointlight[3].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setVec3(" pointlight[3].specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setFloat("pointlight[3].constant", 2.0f);
	shader.setFloat("pointlight[3].linear", 0.09f);
	shader.setFloat("pointlight[3].quadratic", 0.032f);*/
}

void SetupMarryShader(Shader& shader, const RCamera& camera, glm::vec3 lightPosition)
{
	shader.setVec3("eyePos", camera.Location);
	shader.setInteger("emissionV", 2);

	Matrix4 modelTransform(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelTransform));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewTransform()));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(camera.GetPerspective()));

	// material 
	shader.setInteger("material.texture_diffuse1", 0);
	shader.setInteger("material.texture_specular1", 1);
	shader.setFloat("material.shininess", 128.0f);

	// point light 
	shader.setVec3("pointlight.position", lightPosition);
	shader.setVec3("pointlight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointlight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setVec3("pointlight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setFloat("pointlight.constant", 1.0f);
	shader.setFloat("pointlight.linear", 0.09f);
	shader.setFloat("pointlight.quadratic", 0.032f);
}

// the width and height of shadow mapping 
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
void MakeDepthMapFBO(unsigned int& depthMapFBO, unsigned int& depthMap)
{
	// shadow mapping frame buffer object
	glGenFramebuffers(1, &depthMapFBO);

	// ~ Begin shadow mapping texture2D
	glGenTextures(1, &depthMap);

	// Bind depthMap to GL_TEXTURE_2D to setup depthMap.
	glBindTexture(GL_TEXTURE_2D, depthMap);

	// for shadow mapping , no image source.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	// set sample method 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ~End 

	// attach this texture as the framebuffer's depth buffer.
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE); // tell opengl not to render any color data.
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
// meshes
unsigned int planeVAO;
int FALSEmain()
{
	// Init glfw
	glfwInit();
	// Specify gl version 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Gl api version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a display window 
	GLFWwindow* window = glfwCreateWindow(800, 600, "Real Engine", NULL, NULL);
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

	// 隐藏鼠标指针
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 设置指定window 的framebuffer size 的callback.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	glViewport(0, 0, 800, 600);

	// for render light cube .
	Shader lightShader("lightingCube.vert", "light.frag");

	// for marry model
	std::shared_ptr<Shader> modelLoadedShader = Shader::GetShader(EShaderType::ShaderMarry);
	//Shader modelLoadedShader("marry.vert", "marry.frag");

	// for shadow mapping 
	Shader shadowMappingShader("shadowmapping.vert", "shadowmapping.frag");

	// debug depth map shader
	Shader debugDepthShader("debugDepthQuad.vert", "debugDepthQuad.frag");

	// Light object
	RSceneObject lightObj = RSceneObject::Create3DCube();
	RSceneObject::CreateVAO(lightObj);
	lightObj.Translate = glm::vec3(1.2f, 1.0f, 2.0f);
	lightObj.Scale = glm::vec3(0.3f, 0.3f, 0.3f);

	// New model loaded
	RModel marryModel("./resources/objects/mary/Marry.obj");
	
	// plane 
	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f
	};
	// plane VAO
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
	// end plane

	// Wood texture
	unsigned int WoodTexture = Utility::TextureFromFile("wood.png", "./resources/textures");

	// camera
	RCamera Camera;
	KeyEvents.push_back(std::bind(&RCamera::InputEvent, &Camera, std::placeholders::_1, std::placeholders::_2));
	KeyProc.push_back(std::bind(&RCamera::InputProcess, &Camera, std::placeholders::_1, std::placeholders::_2));
	MouseCursorMove.push_back(std::bind(&RCamera::EventMouseMove, &Camera, std::placeholders::_1, std::placeholders::_2));
	MouseScroll.push_back(std::bind(&RCamera::EventMouseScroll, &Camera, std::placeholders::_1, std::placeholders::_2));

	// init debug text render
	TextRender textR("resources/fonts/arial.ttf", glm::vec3(0.5f, 0.8f, 0.2f));
	textRend = &textR;

	glEnable(GL_DEPTH_TEST);

	// for shadow mapping
	unsigned int depthFBO, shadowMapping;
	MakeDepthMapFBO(depthFBO, shadowMapping);

	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightprojection = glm::ortho(-6.0f, 6.0f, -6.0f, 6.0f, near_plane, far_plane);


	auto iden = Matrix4(1.0f);
	float deltaTime = 0.0f, lastFrameTime = 0.0f;
	// glfwGetTime()
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;
		//lightObj.Translate = glm::vec3(sin(glfwGetTime()), 1.0f, cos(glfwGetTime()));
		// 处理指定window 的输入
		processInput(window, deltaTime);



		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 光源cube
		glm::vec3 lightColor(1.0f, 0.5f, 1.0f);
		//lightObj.Translate = glm::vec3(2.f*sinf(glfwGetTime()), 3.5f , 3.0f*cosf(glfwGetTime()))/**sinf(glfwGetTime())*/;
		lightObj.Translate = glm::vec3(2.f, 3.5f , 0.0f)/**sinf(glfwGetTime())*/;

		// Light space transform 
		glm::mat4 lightView = glm::lookAt(lightObj.Translate, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightprojection * lightView;



		shadowMappingShader.use();
		glUniformMatrix4fv(glGetUniformLocation(shadowMappingShader.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			// Plane
			shadowMappingShader.setMat4("model", glm::mat4(1.0f));
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Marry
			glUniformMatrix4fv(glGetUniformLocation(shadowMappingShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(Matrix4(1.0f)));
			marryModel.Draw(shadowMappingShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// End Shadow mapping



		// -------------------------------------------
		// Reset viewport
		glViewport(0, 0, 800, 600);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//debugDepthShader.use();
		//debugDepthShader.setFloat("near_plane", near_plane);
		//debugDepthShader.setFloat("far_plane", far_plane);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, shadowMapping);
		//renderQuad();

		// -----------Scene-------------
		// Light
		lightObj.Scale = glm::vec3(0.3f, 0.3f, 0.3f);
		lightShader.use();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightObj.GetModelTrasform()));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(Camera.GetViewTransform()));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(Camera.GetPerspective()));
		lightShader.setVec3("lightColor", lightColor);
		glBindVertexArray(lightObj.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Render render.Draw(lightObj);

		glBindVertexArray(0);

		// My marry model 
		modelLoadedShader->use();
		SetupMarryShader(*modelLoadedShader, Camera, lightObj.Translate);
		
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, shadowMapping);
		modelLoadedShader->setInteger("shadowmap", 2);
		glUniformMatrix4fv(glGetUniformLocation(modelLoadedShader->ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		//glActiveTexture(GL_TEXTURE0);
		marryModel.Draw(*modelLoadedShader);

		// Floor
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// draw debug text
		textR.DrawOnScreenDebugMessage(deltaTime);

		glfwPollEvents(); // keyboard input, mouse movement events

		glfwSwapBuffers(window); // 交换buffer, 显示最新的渲染结果
	}

	glfwTerminate();

	return 0;
}
/*******************************************************************************************************/
// 对一个三角形, 先 用 vertex shader 计算三个点, 将输出进行插值后输入到 fargment shader


// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
