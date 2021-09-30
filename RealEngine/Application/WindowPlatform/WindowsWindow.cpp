#include "WindowsWindow.h"

#include <sstream>
#include <string>

const wchar_t WindowsWindow::WindowClassName[] = L"RealWindow";

void WindowsWindow::SetTitle(const std::wstring& title)
{
    ::SetWindowText(m_hwnd,title.c_str());
}

void WindowsWindow::Initialize(const GenericWindowDesc& desc, HINSTANCE inInstance)
{
    RECT windowRect = { 0, 0, static_cast<LONG>(desc.width), static_cast<LONG>(desc.height)};
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    m_hwnd = CreateWindow(
        WindowClassName,
        desc.title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr, 
        nullptr, 
        inInstance,
        nullptr);

    ::ShowWindow(m_hwnd, SW_SHOW);
}
