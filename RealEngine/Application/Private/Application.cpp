#include "Application.h"
#include <iostream>

#include "GenericPlatform/GenericPlatformMisc.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWindow.h"

RApplication *RApplication::Get()
{
    static RApplication *single = new RApplication();
    return single;
}

void RApplication::Initilization()
{
    std::cout << __FUNCTION__ << " : Initialization" << std::endl;

    PlatformApp = std::shared_ptr<RGenericApplication>(GenericPlatformMisc::CreatePlatformApplication());

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
