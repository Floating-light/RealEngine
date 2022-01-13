#pragma once 
#include <memory>
class RGenericWindow;

enum class EMouseButton
{
    Left, 
    Middle, 
    Right, 
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
    virtual bool OnMouseButtonDown(const std::shared_ptr<RGenericWindow>& Window, const EMouseButton Button)
    {
        return false;
    }
    virtual bool OnMouseButtonUp(const EMouseButton Button)
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