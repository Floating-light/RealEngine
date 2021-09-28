#pragma once 
#include <windows.h>
#include "../GenericPlatform/GenericWindow.h"

class WindowsWindow: public GenericWindow
{
public:
    int Run(class D3DApp* Graphics, HINSTANCE hInstance, int nCmdShow);
    HWND GetHwnd() { return m_hwnd;}
    void SetWindowTitleText();
protected:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM  lParam);
private:
    HWND m_hwnd;
};