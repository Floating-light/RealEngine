#pragma once 
#include "Core.h"

#include "Input/ApplicationViewport.h"

class RCamera;

class RGameViewportClient : public IAppViewport
{
public:
    RGameViewportClient();
    // ~ IAppViewport
    virtual Reply OnKeyDown( const RGeometry& MyGeometry, const RKeyEvent& InKeyEvent );
	virtual Reply OnKeyUp( const RGeometry& MyGeometry, const RKeyEvent& InKeyEvent );
    virtual Reply OnMouseButtonDown( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent );
	virtual Reply OnMouseButtonUp( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent );
    virtual Reply OnMouseMove( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent );
    // ~ IAppViewport

    void Update();
    void GetViewPoint(Vector& OutLocation, Rotator& OutRotation) const;
    void GetViewTransform(Matrix4& OutMat) const;
    void SetUpView(struct RViewInfo& InOutViewInfo);
private:
    Vector Location = Vector::ZeroVector;
    Rotator Rotation;

    Vector DeltaLocation = Vector::ZeroVector;
    Rotator DeltaRotator{}; 
    
    std::shared_ptr<RCamera> m_Camera;
};