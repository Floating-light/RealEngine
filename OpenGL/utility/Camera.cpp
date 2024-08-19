#include "Camera.h"
#include "GLFW/glfw3.h"

void RCamera::Tick(float DeltaTime)
{

}

void RCamera::InputEvent(int key, int action)
{

}

void RCamera::EventMouseMove(double xpos, double ypos)
{
	float offsetX = (xpos - lastX) * sensitivity;
	float offsetY = (lastY - ypos ) * sensitivity;
	lastX = xpos;
	lastY = ypos;
	if (FirstMouse)
	{
		FirstMouse = false;
		return;
	}
	Rotator.x += offsetY; // Pitch 
	Rotator.y += offsetX; // Yaw
	if (Rotator.x > 89.0f) Rotator.x = 89.0f;
	if (Rotator.x < -89.0f) Rotator.x = -89.0f;
	Front.x = cosf(RMath::Radians(Rotator.y)) * cos(RMath::Radians(Rotator.x));
	Front.y = sinf(RMath::Radians(Rotator.x));
	Front.z = sinf(RMath::Radians(Rotator.y)) * cos(RMath::Radians(Rotator.x));
	Front = RMath::Normalize(Front);
	
	/* 动roll up 就不会是固定的*/
}

void RCamera::EventMouseScroll(double xoffset, double yoffset)
{
	Zoom -= (float)yoffset;
	RMath::Clamp(Zoom, 1.0f, 45.0f);
}

void RCamera::InputProcess(GLFWwindow* window, float Rate)
{
	const float cameraSpeed = 2.5f * Rate;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Location += cameraSpeed * Front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Location -= cameraSpeed * Front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		// 不norm 会基于Front朝向有不同的大小
		Location -= RMath::Normalize(RMath::Cross(Front, Up)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Location += RMath::Normalize(RMath::Cross(Front, Up))* cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		Location += cameraSpeed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		Location -= cameraSpeed * Up;
	}
	//Location.y = 0.0f;
}

