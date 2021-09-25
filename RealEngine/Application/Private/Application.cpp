#include "Application.h"
#include <iostream>

RApplication& RApplication::Get() 
{
    static RApplication single;
    return single; 
}
void RApplication::Initilization()
{
    std::cout << __FUNCTION__ << " : Initialization" << std::endl;
}

std::shared_ptr<RWindow> RApplication::GetMainWindow()
{
    return Windows.at(0);
}
