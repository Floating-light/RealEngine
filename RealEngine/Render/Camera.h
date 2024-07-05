#pragma once

#include "Core.h"
#include "Matrix3.h"

class Camera
{
public:
	Camera() :m_ViewMatrix(Matrix4::Identity)
	{
		SetPrespectiveMatrix(PI / 4.f, 9.0f/16.0f, 1.0f, 1000.0f);
	}
	void SetLookAt(Vector3D Forward, Vector3D Up);
	void SetLocation(const Vector3D Location);
	void Update();
	void SetPrespectiveMatrix(float verticalFovRadians, float aspectHeightOverWidth, float nearZClip, float farZClip); 
private:
	void SetProjMatrix(const Matrix4& InProj);
	void UpdatePrjectMatrix();
private:
	// View 空间
	Matrix3 m_Basis;
	Transform m_CameraToWorld; 
	Matrix4 m_ViewMatrix;
	Matrix4 m_ProjMatrix;
	Matrix4 m_ViewProjMatrix;

	float m_VerticalFOV;
	float m_AspectRatio;
	float m_NearClip;
	float m_FarClip;
	bool m_ReverseZ = false;
	bool m_InfiniteZ = false;
};

inline void Camera::SetProjMatrix(const Matrix4& InProj)
{
	m_ProjMatrix = InProj;
}
