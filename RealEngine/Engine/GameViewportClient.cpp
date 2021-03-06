#include "GameViewportClient.h"
#include "ViewInfo.h"

RGameViewportClient::RGameViewportClient()
{
    
}

Reply RGameViewportClient::OnKeyDown( const RGeometry& MyGeometry, const RKeyEvent& InKeyEvent )
{
    RLOG(INFO) << __FUNCTION__ << " : " << InKeyEvent.GetKey().ToString() ;
    Vector LocalMove;
    if(InKeyEvent.GetKey() == RKey::W)
    {
        LocalMove += Rotation.ToVector()*2;
    }
    else if(InKeyEvent.GetKey() == RKey::S)
    {
        LocalMove -= Rotation.ToVector()*2;
        // LocalMove.X 
    }
    else if(InKeyEvent.GetKey() == RKey::A)
    {
        LocalMove -= RQuat(Rotation).RotateVector(Vector(0.0,1.0,0.0))*2;
    }
    else if(InKeyEvent.GetKey() ==RKey::D)
    {
        LocalMove += RQuat(Rotation).RotateVector(Vector(0.0,1.0,0.0))*2;
    }
    else
    {
        return Reply::Unhandled();
    }
    Location += LocalMove;

    return Reply::Handled();
}
Reply RGameViewportClient::OnKeyUp( const RGeometry& MyGeometry, const RKeyEvent& InKeyEvent )
{
    RLOG(INFO) << __FUNCTION__ << " : " << InKeyEvent.GetKey().ToString() ;

    return Reply::Unhandled();
}
Reply RGameViewportClient::OnMouseButtonDown( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent )
{
    RLOG(INFO) << __FUNCTION__ << " : " << MouseEvent.GetKey().ToString() ;

    return Reply::Unhandled();
}
Reply RGameViewportClient::OnMouseButtonUp( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent )
{
    RLOG(INFO) << __FUNCTION__ << " : " << MouseEvent.GetKey().ToString() ;

    return Reply::Unhandled();
}
Reply RGameViewportClient::OnMouseMove( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent )
{

    return Reply::Unhandled();
}

void RGameViewportClient::SetUpView(struct RViewInfo& InOutViewInfo)
{
    GetViewTransform(InOutViewInfo.ViewMat);
    InOutViewInfo.ProjectionMat.SetIdentity();

    InOutViewInfo.ViewProjectionMat = InOutViewInfo.ViewMat*InOutViewInfo.ProjectionMat;
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

    LookFromMatrix ViewTrans(InLocation, GrazeDirection, Vector(0.0,1.0,0.0));
    OutMat = ViewTrans;
}

void RGameViewportClient::GetViewTransform(Matrix4& OutMat) const
{
    Vector ViewLocation;
    Rotator ViewRotation;
    GetViewPoint(ViewLocation, ViewRotation);

    CalculViewTransform(ViewLocation, ViewRotation, OutMat);
}


