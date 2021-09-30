#include "WindowsApplication.h"
#include "WindowPlatform/WindowsWindow.h"
#include "Core.h"
WindowsApplication* WindowsPlatform = nullptr;
WindowsApplication* WindowsApplication::CreateWindowsApplication(HINSTANCE inInstance,HICON iconHandle)
{
    WindowsPlatform = new WindowsApplication(inInstance, iconHandle);
    return WindowsPlatform;
}

WindowsApplication::WindowsApplication(HINSTANCE inInstance, HICON inIcon)
{
    m_hInstance = inInstance;

    WNDCLASSEX windowsClass = {0};
    windowsClass.cbSize = sizeof(WNDCLASSEX);
    windowsClass.style = CS_HREDRAW | CS_VREDRAW;
    windowsClass.lpfnWndProc = WindowProc;
    windowsClass.hInstance = m_hInstance;
    windowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowsClass.lpszClassName = WindowsWindow::WindowClassName;
    windowsClass.hIcon = inIcon;
    ::RegisterClassEx(&windowsClass);
}

LRESULT CALLBACK WindowsApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            // Save the DXSample* passed in to CreateWindow.
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        }
        return 0;

    case WM_KEYDOWN:
        {
            // pSample->OnKeyDown(static_cast<UINT8>(wParam));
        }
        return 0;

    case WM_KEYUP:
        {
            // pSample->OnKeyUp(static_cast<UINT8>(wParam));
        }
        return 0;

    case WM_PAINT:
        {
            // pSample->OnUpdate();
            // pSample->OnRender();
        }
        return 0;

    case WM_DESTROY:
    {
        RCoreGlobal::SetRequireExit();
        PostQuitMessage(0);
        return 0;
    }
    }

    // Handle any messages the switch statement didn't.
    return DefWindowProc(hWnd, message, wParam, lParam);
}

std::shared_ptr<RGenericWindow> WindowsApplication::MakeWindow() 
{
    return std::make_shared<WindowsWindow>();
}

void WindowsApplication::InitlializeWindow(std::shared_ptr<RGenericWindow> InWindow,const struct GenericWindowDesc& desc) 
{
    std::static_pointer_cast<WindowsWindow>(InWindow)->Initialize(desc,m_hInstance);
}
