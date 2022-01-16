#pragma once 
#include <memory>

#include "Core.h"

class RGenericWindow;

enum class EMouseButton
{
    Left, 
    Middle, 
    Right, 
    Thumb01,
    Thumb02,
    Invalid
};

class GenericApplicationMessageHandler
{
public:
    virtual bool OnKeyDown(const int KeyCode, const int CharacterCode, const bool IsRepeat)
    {
        return false;
    };
    virtual bool OnKeyUp(const int KeyCode, const int CharacterCode, const bool IsRepeat)
    {
        return false;
    }
    virtual bool OnMouseDoubleClick(const std::shared_ptr<RGenericWindow>& Window, const EMouseButton Button, const Vector2D CursorPos)
    {
        return false;
    }
    virtual bool OnMouseButtonDown(const std::shared_ptr<RGenericWindow>& Window, const EMouseButton Button, const Vector2D CursorPos)
    {
        return false;
    }
    virtual bool OnMouseButtonUp(const EMouseButton Button, const Vector2D CursorPos)
    {
        return false;
    }
    virtual bool OnRawMouseMove(const int X, const int Y)
    {
        return false;
    }
    virtual bool OnKeyChar(const wchar_t Character, const bool IsRepeat)
    {
        return false;
    }
};