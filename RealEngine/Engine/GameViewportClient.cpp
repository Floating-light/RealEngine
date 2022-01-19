#include "GameViewportClient.h"

RGameViewportClient::RGameViewportClient()
{
    
}

Reply RGameViewportClient::OnKeyDown( const RGeometry& MyGeometry, const RKeyEvent& InKeyEvent )
{
    RLOG(INFO) << __FUNCTION__ << " : " << InKeyEvent.GetKey().ToString() ;
    return Reply::Unhandled();
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
