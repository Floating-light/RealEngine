#include "Camera.h"

void Camera::SetLookAt(Vector3D Forward, Vector3D Up)
{
	Forward = RMath::Normalize(Forward);
	Vector Right = RMath::Normalize(Vector3D::CrossProduct(Forward, Up));
	Up = Vector3D::CrossProduct(Right, Forward);
	ViewMatrix.SetAxis(0, Right);
	ViewMatrix.SetAxis(1, Up);
	ViewMatrix.SetAxis(2, -Forward);
}

void Camera::SetLocation(const Vector3D Location)
{
	ViewMatrix.SetAxis(3, Location);
}

void Camera::Update()
{

}

void Camera::SetPrespectiveMatrix(float verticalFovRadians, float aspectHeightOverWidth, float nearZClip, float farZClip)
{
	m_VerticalFOV = verticalFovRadians;
	m_AspectRatio = aspectHeightOverWidth;
	m_NearClip = nearZClip;
	m_FarClip = farZClip;

	UpdatePrjectMatrix();
}

void Camera::UpdatePrjectMatrix()
{
	const float Y = 1.0f / std::tanf(m_VerticalFOV * 0.5f);
	const float X = Y * m_AspectRatio;

	float Q1, Q2;
	if(m_ReverseZ)
	{

	}
	else
	{
		if(m_InfiniteZ)
		{

		}
		else
		{
			Q1 = m_FarClip / (m_NearClip - m_FarClip);
			Q2 = Q1 * m_NearClip;
		}
	}
	;
	SetProjMatrix(Matrix4(
		RVec
	));
}


