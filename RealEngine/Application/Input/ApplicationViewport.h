#pragma once 
#include "Reply.h"
#include "Events.h"

class RGeometry{};
class IAppViewport
{
public:
	virtual ~IAppViewport() {} ;
	virtual Reply OnMouseButtonDown( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent )
	{
		return Reply::Unhandled();
	}

	virtual Reply OnMouseButtonUp( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent )
	{
		return Reply::Unhandled();
	}

	virtual void OnMouseEnter( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent )
	{

	}

	virtual void OnMouseLeave( const RPointerEvent& MouseEvent )
	{
		
	}

	virtual Reply OnMouseMove( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent )
	{
		return Reply::Unhandled();
	}

	virtual Reply OnMouseWheel( const RGeometry& MyGeometry, const RPointerEvent& MouseEvent )
	{
		return Reply::Unhandled();
	}

	virtual Reply OnMouseButtonDoubleClick( const RGeometry& InMyGeometry, const RPointerEvent& InMouseEvent )
	{
		return Reply::Unhandled();
	}

	virtual Reply OnKeyDown( const RGeometry& MyGeometry, const RKeyEvent& InKeyEvent )
	{
		return Reply::Unhandled();
	}

	virtual Reply OnKeyUp( const RGeometry& MyGeometry, const RKeyEvent& InKeyEvent )
	{
		return Reply::Unhandled();
	}
};