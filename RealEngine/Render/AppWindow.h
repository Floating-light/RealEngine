#pragma once 
#include <windows.h>
class AppWindow
{
public:
    static AppWindow& Get();
    int Run(class D3DApp* Graphics, HINSTANCE hInstance, int nCmdShow);
    HWND GetHwnd() { return m_hwnd;}
protected:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM  lParam);
private:
    HWND m_hwnd;
};