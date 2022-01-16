#include "Application.h"
#include <iostream>

#include "GenericPlatform/GenericPlatformMisc.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWindow.h"
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
    RLOG(INFO) << __FUNCTION__ << ", KeyCode : " << KeyCode << ", CharacterCode : " << CharacterCode ;
    return false;
}
bool RApplication::OnKeyUp(const int KeyCode, const int CharacterCode, const bool IsRepeat)
{
    RLOG(INFO) << __FUNCTION__ << ", KeyCode : " << KeyCode << ", CharacterCode : " << CharacterCode ;
    return false;
}
bool RApplication::OnMouseButtonDown(const std::shared_ptr<RGenericWindow>& Window, const EMouseButton Button, const Vector2D CursorPos)
{
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