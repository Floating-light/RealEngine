#include "WindowsApplication.h"
#include "WindowPlatform/WindowsWindow.h"

WindowsApplication* WindowsPlatform = nullptr;
const wchar_t WindowClassName[] = L"RealWindow";
WindowsApplication* WindowsApplication::CreateWindowsApplication(HINSTANCE inInstance,HICON iconHandle)
{
    WindowsPlatform = new WindowsApplication(inInstance, iconHandle);
    return WindowsPlatform;
}

WindowsApplication::WindowsApplication(HINSTANCE inInstance, HICON inIcon)
{
    hInstance = inInstance;

    WNDCLASSEX windowsClass = {0};
    windowsClass.cbSize = sizeof(WNDCLASSEX);
    windowsClass.style = CS_HREDRAW | CS_VREDRAW;
    windowsClass.lpfnWndProc = WindowProc;
    windowsClass.hInstance = inInstance;
    windowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowsClass.lpszClassName = WindowClassName;
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
        PostQuitMessage(0);
        return 0;
    }

    // Handle any messages the switch statement didn't.
    return DefWindowProc(hWnd, message, wParam, lParam);
}

std::shared_ptr<RGenericWindow> WindowsApplication::MakeWindow() 
{

}

void WindowsApplication::InitlializeWindow(std::shared_ptr<RGenericWindow> InWindow,const struct GenericWindowDesc& desc) 
{

}
