#include "Camera.h"

void RCamera::SetTransform(const Vector& Location, const Rotator& Rotation)
{
	Matrix3 rot = Matrix3::MakeYRotation(Rotation.Yaw) * Matrix3::MakeXRotation(Rotation.Pitch);

	SetLookAt(-rot.GetZ(), rot.GetY());
	SetLocation(Location);
}

void RCamera::SetLookAt(Vector3D Forward, Vector3D Up)
{
	Forward = RMath::Normalize(Forward);
	Vector Right = RMath::Normalize(Vector3D::CrossProduct(Forward, Up));
	Up = Vector3D::CrossProduct(Right, Forward);
	m_Basis.SetX(Right);
	m_Basis.SetY(Up);
	m_Basis.SetZ(-Forward);

	m_CameraToWorld.SetRatation(RQuat(m_Basis));
}

void RCamera::SetLocation(const Vector3D Location)
{
	m_CameraToWorld.SetTranslation(Location);
}

void RCamera::Update()
{
	m_ViewMatrix = (~m_CameraToWorld).ToMatrixNoScale(); 

	m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
}

void RCamera::SetPrespectiveMatrix(float verticalFovRadians, float aspectHeightOverWidth, float nearZClip, float farZClip)
{
	m_VerticalFOV = verticalFovRadians;
	m_AspectRatio = aspectHeightOverWidth;
	m_NearClip = nearZClip;
	m_FarClip = farZClip;

	UpdatePrjectMatrix();
}

void RCamera::UpdatePrjectMatrix()
{
	const float Y = 1.0f / std::tanf(m_VerticalFOV * 0.5f);
	const float X = Y * m_AspectRatio;

	float Q1, Q2;
	if(m_ReverseZ)
	{
		if (m_InfiniteZ)
		{
			Q1 = 0.0f;
			Q2 = m_NearClip;
		}
		else
		{
			Q1 = m_NearClip / (m_FarClip - m_NearClip);
			Q2 = Q1 * m_FarClip;
		}
	}
	else
	{
		if(m_InfiniteZ)
		{
			Q1 = -1.0f;
			Q2 = -m_NearClip;
		}
		else
		{
			Q1 = m_FarClip / (m_NearClip - m_FarClip);
			Q2 = Q1 * m_NearClip;
		}
	}
	;
	SetProjMatrix(Matrix4(
		RVector4D(X, 0.0f, 0.0f, 0.0f),
		RVector4D(0.0f, Y, 0.0f, 0.0f),
		RVector4D(0.0f, 0.0f, Q1, -1.0f),
		RVector4D(0.0f, 0.0f, Q2, 0.0f)
	));
}


