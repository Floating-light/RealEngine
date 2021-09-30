#pragma once 
#include <windows.h>
#include "../GenericPlatform/GenericWindow.h"

class WindowsWindow: public RGenericWindow
{
public:

    virtual ~WindowsWindow(){};
    virtual void* GetWindowHandle() override {return m_hwnd;};
    virtual void SetTitle(const std::wstring& title) override ;
    
    void Initialize(const GenericWindowDesc& desc, HINSTANCE inInstance);
public:
    static const wchar_t WindowClassName[];
private:
    HWND m_hwnd;
};