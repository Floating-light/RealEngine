#pragma once

#include <windows.h>
#include "GenericPlatform/GenericApplication.h"

class WindowsApplication : public RGenericApplication
{
public:
    virtual std::shared_ptr<RGenericWindow> MakeWindow() override;    
    virtual void InitlializeWindow(std::shared_ptr<RGenericWindow> InWindow,const struct GenericWindowDesc& desc) override;

    static WindowsApplication* CreateWindowsApplication(HINSTANCE inInstance,HICON iconHandle);

private:
    WindowsApplication(HINSTANCE inInstance, HICON inIcon);
    
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HINSTANCE m_hInstance;
};