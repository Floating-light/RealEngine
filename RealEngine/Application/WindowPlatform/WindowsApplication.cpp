#include "WindowsApplication.h"

#include <iostream>
#include <bitset>
#include <windowsx.h>

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
    case WM_SYSKEYDOWN: // Key down or repeat 
    case WM_KEYDOWN:
        {
            const int Win32Key = wParam;

            int ActualKey = Win32Key;
            bool bIsRepeat = (lParam & 0x40000000 ) != 0;
            switch(Win32Key)
            {
            case VK_MENU: // Left and right alt
                if(lParam & 0x1000000 == 0 )
                {
                    ActualKey = VK_LMENU;
                    bIsRepeat = ModifierKeyState[EModifierKey::LeftAlt];
                    ModifierKeyState[EModifierKey::LeftAlt] = true;
                }
                else
                {
                    ActualKey = VK_RMENU;
                    bIsRepeat = ModifierKeyState[EModifierKey::RightAlt];
                    ModifierKeyState[EModifierKey::RightAlt] = true;
                }
                break;
            case VK_CONTROL:
                if(lParam & 0x1000000 == 0 )
                {
                    ActualKey = VK_LCONTROL;
                    bIsRepeat = ModifierKeyState[EModifierKey::LeftControl];
                    ModifierKeyState[EModifierKey::LeftControl] = true;
                }
                else
                {
                    ActualKey = VK_RCONTROL;
                    bIsRepeat = ModifierKeyState[EModifierKey::RightControl];
                    ModifierKeyState[EModifierKey::RightControl] = true;
                }
                break;
            case VK_SHIFT:
                ActualKey = MapVirtualKey((lParam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
                if(ActualKey == VK_LSHIFT)
                {
                    bIsRepeat = ModifierKeyState[EModifierKey::LeftShift];
                    ModifierKeyState[EModifierKey::LeftShift] = true;
                }
                else
                {
                    bIsRepeat = ModifierKeyState[EModifierKey::RightShift];
                    ModifierKeyState[EModifierKey::RightShift] = true;
                }
                break;
            case VK_CAPITAL:
                ModifierKeyState[EModifierKey::CapsLock] = (::GetKeyState(VK_CAPITAL) && 0x0001) != 0;
                break;
            default:
                break;
            }
            
            // return 0 if not translation from virtual key to character exists.
            int CharCode = ::MapVirtualKey(Win32Key, MAPVK_VK_TO_CHAR);

            const bool Result = MessageHandler->OnKeyDown(ActualKey, CharCode, bIsRepeat);
            if(Result || message != WM_SYSKEYDOWN)
            {
                return 0;
            }
            // pSample->OnKeyDown(static_cast<UINT8>(wParam));
        }
        break;
    case WM_SYSKEYUP: // Key up 
    case WM_KEYUP:
        {
            // Character code is stored in WPARAM
			int Win32Key = wParam;

			// The actual key to use.  Some keys will be translated into other keys. 
			// I.E VK_CONTROL will be translated to either VK_LCONTROL or VK_RCONTROL as these
			// keys are never sent on their own
			int ActualKey = Win32Key;

			bool bModifierKeyReleased = false;
			switch( Win32Key )
			{
			case VK_MENU:
				// Differentiate between left and right alt
				if( (lParam & 0x1000000) == 0 )
				{
					ActualKey = VK_LMENU;
					ModifierKeyState[EModifierKey::LeftAlt] = false;
				}
				else
				{
					ActualKey = VK_RMENU;
					ModifierKeyState[EModifierKey::RightAlt] = false;
				}
				break;
			case VK_CONTROL:
				// Differentiate between left and right control
				if( (lParam & 0x1000000) == 0 )
				{
					ActualKey = VK_LCONTROL;
					ModifierKeyState[EModifierKey::LeftControl] = false;
				}
				else
				{
					ActualKey = VK_RCONTROL;
					ModifierKeyState[EModifierKey::RightControl] = false;
				}
				break;
			case VK_SHIFT:
				// Differentiate between left and right shift
				ActualKey = MapVirtualKey( (lParam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
				if (ActualKey == VK_LSHIFT)
				{
					ModifierKeyState[EModifierKey::LeftShift] = false;
				}
				else
				{
					ModifierKeyState[EModifierKey::RightShift] = false;
				}
				break;
			case VK_CAPITAL:
				ModifierKeyState[EModifierKey::CapsLock] = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
				break;
			default:
				// No translation needed
				break;
			}
            // return 0 if not translation from virtual key to character exists.
            const int CharCode = ::MapVirtualKey(Win32Key, MAPVK_VK_TO_CHAR);
            const bool bIsRepeat = false;
            const bool Result = MessageHandler->OnKeyUp(ActualKey, CharCode, bIsRepeat);
            if(Result || message != WM_SYSKEYUP)
            {
                return 0;
            }
        }
        break;
    case WM_INPUT:
        {
            RLOG(INFO) << "Input message " ;
        }
        return 0;
    case WM_CHAR: // Char
        {
            const bool bIsRepeat = (lParam & 0x4000000) != 0;
            MessageHandler->OnKeyChar(wParam, bIsRepeat);
            // RLOG(INFO) << "WM CHAR :" << message << ", wParam : " << wParam << ", lParam : " << std::bitset<8*sizeof(__int64)>(lParam);
            return 0;
        }
        break;
    case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_XBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONUP: // Mouse button down up doubleclick
        {
            POINT CursorPoint;
            CursorPoint.x = GET_X_LPARAM(lParam);
            CursorPoint.y = GET_Y_LPARAM(lParam);
            ::ClientToScreen(hWnd, &CursorPoint);

            const Vector2D CursorPos(CursorPoint.x, CursorPoint.y);

            EMouseButton MouseButtonType = EMouseButton::Invalid;

            bool bDoubleClick = false;
            bool bMouseUp = false;

            switch (message)
            {
            case WM_LBUTTONDBLCLK:
                bDoubleClick = true;
                MouseButtonType = EMouseButton::Left;
                break;
            case WM_LBUTTONUP:
                bMouseUp = true;
                MouseButtonType = EMouseButton::Left;
                break;
            case WM_LBUTTONDOWN:
                MouseButtonType = EMouseButton::Left;
                break;
            case WM_MBUTTONDBLCLK:
                bDoubleClick = true;
                MouseButtonType = EMouseButton::Middle;
                break;
            case WM_MBUTTONUP:
                bMouseUp = true ;
                MouseButtonType = EMouseButton::Middle;
                break;
            case WM_MBUTTONDOWN: 
                MouseButtonType = EMouseButton::Middle;
                break;
            case WM_RBUTTONDBLCLK:
                bDoubleClick = true;
                MouseButtonType = EMouseButton::Right;
                break;
            case WM_RBUTTONUP:
                bMouseUp = true;
                MouseButtonType = EMouseButton::Right;
                break;
            case WM_RBUTTONDOWN:
                MouseButtonType = EMouseButton::Right;
                break;
            case WM_XBUTTONDBLCLK:
                bDoubleClick = true;
                MouseButtonType = (HIWORD(wParam) & XBUTTON1) ? EMouseButton::Thumb01 : EMouseButton::Thumb02;
                break;
            case WM_XBUTTONUP:
                bMouseUp = true;
                MouseButtonType = (HIWORD(wParam) & XBUTTON1) ? EMouseButton::Thumb01 : EMouseButton::Thumb02;
                break;
            case WM_XBUTTONDOWN:
                MouseButtonType = (HIWORD(wParam) & XBUTTON1) ? EMouseButton::Thumb01 : EMouseButton::Thumb02;
                break;
            default:
                CHECK(0);
            }
            if(bMouseUp)
            {
                return MessageHandler->OnMouseButtonUp(MouseButtonType, CursorPos) ? 0 : 1;
            }
            else if (bDoubleClick)
            {
                MessageHandler->OnMouseDoubleClick(CurrentEventWindow, MouseButtonType, CursorPos);
            }
            else
            {
                MessageHandler->OnMouseButtonDown(CurrentEventWindow, MouseButtonType, CursorPos);
            }
            return 0;
        }
        break;
    case WM_NCMOUSEMOVE:
    case WM_MOUSEMOVE:
        {
            POINT CursorPos;
            ::GetCursorPos(&CursorPos);
            return MessageHandler->OnRawMouseMove(CursorPos.x, CursorPos.y);
        }
        break;
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
