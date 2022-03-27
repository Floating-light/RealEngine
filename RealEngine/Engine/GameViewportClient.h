#pragma once 
#include "Input/ApplicationViewport.h"

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

    void GetViewPoint(Vector& OutLocation, Rotator& OutRotation) const;
    void GetViewProjTransform(Matrix4& OutMat) const;
    
private:
    Vector Location;
    Rotator Rotation;
};