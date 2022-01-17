#pragma once 
#include "Reply.h"
#include "Events.h"

class FGeometry{};
class IAppViewport
{
	virtual Reply OnMouseButtonDown( const FGeometry& MyGeometry, const RPointerEvent& MouseEvent )
	{
		return Reply::Unhandled();
	}

	virtual Reply OnMouseButtonUp( const FGeometry& MyGeometry, const RPointerEvent& MouseEvent )
	{
		return Reply::Unhandled();
	}

	virtual void OnMouseEnter( const FGeometry& MyGeometry, const RPointerEvent& MouseEvent )
	{

	}

	virtual void OnMouseLeave( const RPointerEvent& MouseEvent )
	{
		
	}

	virtual Reply OnMouseMove( const FGeometry& MyGeometry, const RPointerEvent& MouseEvent )
	{
		return Reply::Unhandled();
	}

	virtual Reply OnMouseWheel( const FGeometry& MyGeometry, const RPointerEvent& MouseEvent )
	{
		return Reply::Unhandled();
	}

	virtual Reply OnMouseButtonDoubleClick( const FGeometry& InMyGeometry, const RPointerEvent& InMouseEvent )
	{
		return Reply::Unhandled();
	}

	virtual Reply OnKeyDown( const FGeometry& MyGeometry, const RKeyEvent& InKeyEvent )
	{
		return Reply::Unhandled();
	}

	virtual Reply OnKeyUp( const FGeometry& MyGeometry, const RKeyEvent& InKeyEvent )
	{
		return Reply::Unhandled();
	}
};