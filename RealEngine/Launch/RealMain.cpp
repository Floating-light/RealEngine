#include "RealMain.h"
#include "Application.h"
int RealLuanch::Run(const wchar_t* Cmd)
{
    RApplication::Get().Initilization();
    return 0;
}


int RealMain(const wchar_t* Cmd)
{
    return  RealLuanch().Run(Cmd);
}