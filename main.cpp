#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "utility/Render.h"
#include "utility/SceneObject.h"
#include "Shaders/Shader.h"
#include "utility/Model.h"
#include "utility/Camera.h"
#include "utility/LightObject.h"
#include "utility/CoreType.h"

namespace R
{
	std::vector<KeyEventCallback> KeyEvents;
	std::vector<KeyProcess> KeyProc;
	std::vector<MouseCursorMoveCallBack> MouseCursorMove;
	std::vector<MouseScrollCallBack> MouseScroll;
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
		for (auto& item : KeyProc)
		{
			item(window, DeltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
	}
}
// Color(0.828, 0.364, 0.0)
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
	GLFWwindow* window = glfwCreateWindow(800, 600, "RealEngine", NULL, NULL);
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
	glfwSetFramebufferSizeCallback(window, R::framebuffer_size_callback);
	glfwSetKeyCallback(window, R::key_callback);
	glfwSetCursorPosCallback(window, R::mouse_callback);
	glfwSetScrollCallback(window, R::mouse_scroll_callback);

	glViewport(0, 0, 800, 600);

	std::vector<std::shared_ptr<RSceneObject>> SceneObjects;
	// Setup scene 
	{
		std::shared_ptr<RModel> marryModel = std::make_shared<RModel>("./resources/objects/mary/Marry.obj", EShaderType::ShaderMarry);
		std::shared_ptr<RSceneObject> obj1 = std::make_shared<RSceneObject>(marryModel);
		
		SceneObjects.push_back(obj1);
	}

	// Camera 
	std::shared_ptr<RCamera> Camera = std::make_shared<RCamera>();
	R::KeyEvents.push_back(std::bind(&RCamera::InputEvent, Camera, std::placeholders::_1, std::placeholders::_2));
	R::KeyProc.push_back(std::bind(&RCamera::InputProcess, Camera, std::placeholders::_1, std::placeholders::_2));
	R::MouseCursorMove.push_back(std::bind(&RCamera::EventMouseMove, Camera, std::placeholders::_1, std::placeholders::_2));
	R::MouseScroll.push_back(std::bind(&RCamera::EventMouseScroll, Camera, std::placeholders::_1, std::placeholders::_2));

	// Light
	std::shared_ptr<RLightObject> Light = std::make_shared<RLightObject>();

	RRenderer render;
	render.SetCamera(Camera);
	render.SetLight(Light);
	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f, lastFrameTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;
		R::processInput(window, deltaTime);

		glViewport(0, 0, 800, 600);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render.Render(SceneObjects);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}