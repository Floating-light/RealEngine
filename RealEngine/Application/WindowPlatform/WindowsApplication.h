#pragma once
#include <vector>
#include <memory>
#include <windows.h>

#include "GenericPlatform/GenericApplication.h"
class WindowsWindow;

class WindowsApplication : public RGenericApplication
{
public:
    virtual std::shared_ptr<RGenericWindow> MakeWindow() override;    
    virtual void InitlializeWindow(std::shared_ptr<RGenericWindow> InWindow,const struct GenericWindowDesc& desc) override;

    static WindowsApplication* CreateWindowsApplication(HINSTANCE inInstance,HICON iconHandle);

private:
    WindowsApplication(HINSTANCE inInstance, HICON inIcon);
    int ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HINSTANCE m_hInstance;
    enum EModifierKey : uint8_t
    {
        LeftShift,		// VK_LSHIFT
		RightShift,		// VK_RSHIFT
		LeftControl,	// VK_LCONTROL
		RightControl,	// VK_RCONTROL
		LeftAlt,		// VK_LMENU
		RightAlt,		// VK_RMENU
		CapsLock,		// VK_CAPITAL
		Count,
    };
    bool ModifierKeyState[EModifierKey::Count];
    std::vector<std::shared_ptr<WindowsWindow>> Windows;
};