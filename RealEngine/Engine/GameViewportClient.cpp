#include "GameViewportClient.h"
#include "ViewInfo.h"
#include "Camera.h"

RGameViewportClient::RGameViewportClient() : 
    m_Camera(std::shared_ptr<RCamera>(new RCamera())) 
{
    Location = Vector(0, 16.8, 13.38);
}

Reply RGameViewportClient::OnKeyDown( const RGeometry& MyGeometry, const RKeyEvent& InKeyEvent )
{
    RLOG(Info, "{} : {}", __FUNCTION__, InKeyEvent.GetKey().ToString());
    static constexpr float Intensity = 0.06;
    if(InKeyEvent.GetKey() == RKey::W)
    {
        DeltaLocation.Z -= Intensity;
    }
    else if(InKeyEvent.GetKey() == RKey::S)
    {
        DeltaLocation.Z += Intensity;
    }
    else if(InKeyEvent.GetKey() == RKey::A)
    {
        DeltaLocation.X -= Intensity;
    }
    else if(InKeyEvent.GetKey() ==RKey::D)
    {
        DeltaLocation.X += Intensity;
    }
    else if (InKeyEvent.GetKey() == RKey::E)
    {
        DeltaLocation.Y -= Intensity;
    }
    else if (InKeyEvent.GetKey() == RKey::Q)
    {
        DeltaLocation.Y += Intensity;
    }
    else
    {
        return Reply::Unhandled();
    }

    return Reply::Handled();
}
Reply RGameViewportClient::OnKeyUp( const RGeometry& MyGeometry, const RKeyEvent& InKeyEvent )
{
    RLOG(Info, "{} : {}", __FUNCTION__,  InKeyEvent.GetKey().ToString());
    return Reply::Unhandled();
}
Reply RGameViewportClient::OnMouseButtonDown( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent )
{
    RLOG(Info, "{} : {}", __FUNCTION__, MouseEvent.GetKey().ToString());
    return Reply::Unhandled();
}
Reply RGameViewportClient::OnMouseButtonUp( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent )
{
    RLOG(Info, "{} : {}", __FUNCTION__, MouseEvent.GetKey().ToString());
    return Reply::Unhandled();
}
Reply RGameViewportClient::OnMouseMove( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent )
{

    return Reply::Unhandled();
}

void RGameViewportClient::Update()
{
    Matrix3 rot = Matrix3::MakeYRotation(Rotation.Yaw) * Matrix3::MakeXRotation(Rotation.Pitch);
    Location += rot * DeltaLocation;
    m_Camera->SetTransform(Location, Rotation);
    m_Camera->Update();
    RLOG(Info, "{} Camera location: {}", __FUNCTION__, Location.ToString()); 

    DeltaLocation = Vector::ZeroVector;
}

void RGameViewportClient::SetUpView(struct RViewInfo& InOutViewInfo)
{
    InOutViewInfo.SetViewProjectionMatrix(m_Camera->GetViewProjectionMatrix());
}

void RGameViewportClient::GetViewPoint(Vector& OutLocation, Rotator& OutRotation) const
{
    OutLocation = Location;
    OutRotation = Rotation;
}

void CalculViewTransform(const Vector& InLocation, const Rotator& InRotation, Matrix4& OutMat)
{
	// Remove winding and clamp to [-360, 360]
	const float PitchNoWinding = RMath::Radians(RMath::Fmod(InRotation.Pitch, 360.0f));
	const float YawNoWinding = RMath::Radians(RMath::Fmod(InRotation.Yaw, 360.0f));

	float CP = cosf(PitchNoWinding), 
          SP = sinf(PitchNoWinding), 
          CY= cosf(YawNoWinding), 
          SY = sinf(YawNoWinding);

	Vector GrazeDirection( CP*CY, CP*SY, SP );

    //LookFromMatrix ViewTrans(InLocation, GrazeDirection, Vector(0.0,1.0,0.0));
    //OutMat = ViewTrans;
    OutMat = Matrix4::Identity;
}

void RGameViewportClient::GetViewTransform(Matrix4& OutMat) const
{
    Vector ViewLocation;
    Rotator ViewRotation;
    GetViewPoint(ViewLocation, ViewRotation);

    CalculViewTransform(ViewLocation, ViewRotation, OutMat);
}


