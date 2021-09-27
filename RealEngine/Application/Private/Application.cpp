#include "Application.h"
#include <iostream>

#include "GenericPlatform/GenericPlatformFactory.h"
#include "GenericPlatform/GenericApplication.h"

RApplication& RApplication::Get() 
{
    static RApplication single;
    return single; 
}
void RApplication::Initilization()
{
    std::cout << __FUNCTION__ << " : Initialization" << std::endl;
    PlatformApp = std::make_shared<GenericApplication>(GenericPlatformFactory::CreatePlatformApplication());
    
    GenericWindow = PlatformApp->MakeWindow();
}

std::shared_ptr<RWindow> RApplication::GetMainWindow()
{
    return Windows.at(0);
}
