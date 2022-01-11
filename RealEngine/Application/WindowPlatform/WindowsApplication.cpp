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
static std::shared_ptr<WindowsWindow> FindWindowByHandle(const std::vector<std::shared_ptr<WindowsWindow>> WindowsToSearch, HWND hWnd)
{
    for(const auto Item : WindowsToSearch)
    {
        if(Item->GetWindowHandle() == hWnd)
        {
            return Item;
        }
    }
    return nullptr;
}
LRESULT CALLBACK WindowsApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return WindowsPlatform->ProcessMessage(hWnd, message, wParam, lParam);
}
int WindowsApplication::ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    std::shared_ptr<WindowsWindow> CurrentEventWindow = FindWindowByHandle(Windows, hWnd);
    if(!CurrentEventWindow)
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    
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
            RLOG(INFO) << "Message : " << message << ", wParam : " << wParam << ", lParam : " << lParam;
            // pSample->OnKeyDown(static_cast<UINT8>(wParam));
        }
        return 0;

    case WM_KEYUP:
        {
            RLOG(INFO) << "Message : " << message << ", wParam : " << wParam << ", lParam : " << lParam;
            // pSample->OnKeyUp(static_cast<UINT8>(wParam));
        }
        return 0;

    case WM_PAINT:
        {
            // pSample->OnUpdate();
            // pSample->OnRender();
        }
        break;

    case WM_DESTROY:
    {
        RCoreGlobal::SetRequireExit();
        PostQuitMessage(0);
        return 0;
    }
    }

    // Handle any messages the switch statement didn't.
    return DefWindowProc(hWnd, message, wParam, lParam);
    // return 0;
}

std::shared_ptr<RGenericWindow> WindowsApplication::MakeWindow() 
{
    return std::make_shared<WindowsWindow>();
}

void WindowsApplication::InitlializeWindow(std::shared_ptr<RGenericWindow> InWindow,const struct GenericWindowDesc& desc) 
{
    const auto InWindowsWind = std::static_pointer_cast<WindowsWindow>(InWindow);
    Windows.push_back(InWindowsWind);
    InWindowsWind->Initialize(desc,m_hInstance);
}
