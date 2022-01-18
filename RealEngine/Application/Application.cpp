#include "Application.h"
#include <iostream>

#include "GenericPlatform/GenericPlatformMisc.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWindow.h"
#include "Input/InputKeyManager.h"
#include "Input/Events.h"
std::shared_ptr<RApplication> RApplication::Application = nullptr;

void RApplication::Create()
{
    Application = std::make_shared<RApplication>();
    Application->Initilization();
}

RApplication& RApplication::Get()
{
    CHECK(Application) << "Application is nullptr";
    return *Application;
}

void RApplication::Initilization()
{
    PlatformApp = std::shared_ptr<RGenericApplication>(GenericPlatformMisc::CreatePlatformApplication());
    PlatformApp->SetMessageHandler(Application);

    MainWindow = PlatformApp->MakeWindow();
    Windows.push_back(MainWindow);

    GenericWindowDesc desc;
    desc.height = 720;
    desc.width = 1280;
    desc.title = L"RealEngine";
    PlatformApp->InitlializeWindow(MainWindow, desc);
}

std::shared_ptr<RGenericWindow> RApplication::GetMainWindow()
{
    return MainWindow;
}

void RApplication::ProcessInput()
{
    GenericPlatformMisc::PumpMessages();
}

bool RApplication::OnKeyDown(const int KeyCode, const int CharacterCode, const bool IsRepeat)
{
    RKey Key = RInputKeyManager::Get().GetKeyFromCodes(KeyCode, CharacterCode);
    RKeyEvent Event(Key,PlatformApp->GetModifierKeys(),IsRepeat, KeyCode, CharacterCode);
    
    RLOG(INFO) << __FUNCTION__ << "Key down : " << Event.GetKey().ToString() << ",  Is repeat : " << Event.IsRepeat() ;
    return false;
}

bool RApplication::OnKeyUp(const int KeyCode, const int CharacterCode, const bool IsRepeat)
{
    RKey Key = RInputKeyManager::Get().GetKeyFromCodes(KeyCode, CharacterCode);
    RKeyEvent Event(Key,PlatformApp->GetModifierKeys(),IsRepeat, KeyCode, CharacterCode);

    RLOG(INFO) << __FUNCTION__ << "Key uP : " << Event.GetKey().ToString() << ",  Is repeat : " << Event.IsRepeat() ;

    return false;
}

static RKey TranslateMouseButtonToKey(const EMouseButton Button)
{
    RKey Key = RKey::Invalid;
    switch (Button)
    {
    case EMouseButton::Left:
        Key = RKey::LeftMouseButton;
        break;
    case EMouseButton::Middle:
        Key = RKey::MiddleMouseButton;
        break;
    case EMouseButton::Right:
        Key = RKey::RightMouseButton;
        break;
    case EMouseButton::Thumb01:
        Key = RKey::ThumbMouseButton;
        break;
    case EMouseButton::Thumb02:
        Key = RKey::ThumbMouseButton2;
        break;
    }
    return Key;
}
bool RApplication::OnMouseButtonDown(const std::shared_ptr<RGenericWindow>& Window, const EMouseButton Button, const Vector2D CursorPos)
{
    RKey Key = TranslateMouseButtonToKey(Button);
    // RPointerEvent MouseEvent();
    RLOG(INFO) << __FUNCTION__ << ", Button " << static_cast<int>(Button) << ", Position " << CursorPos.ToString();
    return false;
}
bool RApplication::OnMouseButtonUp(const EMouseButton Button, const Vector2D CursorPos)
{
    RLOG(INFO) << __FUNCTION__ << ", Button " << static_cast<int>(Button)<< ", Position " << CursorPos.ToString();
    return false;
}
bool RApplication::OnRawMouseMove(const int X, const int Y)
{
    RLOG(INFO) << __FUNCTION__ << ",  (X ,y) " << X << ", " << Y;
    return false;
}