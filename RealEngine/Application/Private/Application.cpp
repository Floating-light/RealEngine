#include "Application.h"
#include <iostream>

#include "GenericPlatform/GenericPlatformFactory.h"
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
    PlatformApp = std::make_shared<RGenericApplication>(GenericPlatformFactory::CreatePlatformApplication());

    MainWindow = PlatformApp->MakeWindow();
    Windows.push_back(MainWindow);

    GenericWindowDesc desc;

    PlatformApp->InitlializeWindow(MainWindow, desc);
}

std::shared_ptr<RGenericWindow> RApplication::GetMainWindow()
{
    return MainWindow;
}