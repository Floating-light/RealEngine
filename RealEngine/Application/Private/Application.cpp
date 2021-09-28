#include "Application.h"
#include <iostream>

#include "GenericPlatform/GenericPlatformFactory.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/GenericWindow.h"

RApplication& RApplication::Get() 
{
    static RApplication single;
    return single; 
}
void RApplication::Initilization()
{
    std::cout << __FUNCTION__ << " : Initialization" << std::endl;
    PlatformApp = std::make_shared<RGenericApplication>(GenericPlatformFactory::CreatePlatformApplication());
    
    MainWindow = PlatformApp->MakeWindow();
    PlatformApp->Initialize(MainWindow,);
}

std::shared_ptr<RGenericWindow> RApplication::GetMainWindow()
{
    return Windows.at(0);
}
