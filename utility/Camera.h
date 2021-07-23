#pragma once 
#include "Math.h"
#include "utility/SceneObject.h"

class RCamera : public RSceneObject
{
public:
	RCamera() :
		Location(0.0f, 0.0f, 3.0f),
		Front(0.0f, 0.0f, -1.0f),
		Up(0.0f, 1.0f, 0.0f),
		RotationRadius(10.0f),
		FirstMouse(true),
		sensitivity(0.05f),
		Zoom(45.f),
		Rotator(0.0f, 0.0f,0.0f),
		lastX(400.0f),
		lastY(300.0f)
	{

	}

	virtual void Tick(float DeltaTime) override;

	void InputEvent(int key, int action);
	
	void InputProcess(GLFWwindow* window, float Rate);
	void EventMouseMove(double xpos, double ypos);
	void EventMouseScroll(double xoffset, double yoffset);
	
	Matrix4 GetViewTransform() const
	{
		return RMath::ViewTransform(Location, Location + Front, Up);
	}

	Matrix4 GetPerspective() const 
	{
		return RMath::PerspectiveTransform(RMath::Radians(Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
	}

	// The location of this camera, world coordinates
	Vector3D Location;

	// The front direction vector, norm.
	Vector3D Front; // ³¯Ïò ?

	// The Up to use for caculate right and real Up vector.
	Vector3D Up;

	// Pitch Yaw Roll for rotate Front direction, not roll, 
	Vector3D Rotator;// pitch Yaw roll 7

private:
	float RotationRadius;
	// Helper variable to indecate if first time focus
	bool FirstMouse;
	// The last cursor cooridinate
	float lastX, lastY;
	// Rotate sensitivity
	float sensitivity;
	// Camera zoom , actually is fov.
	float Zoom;
};