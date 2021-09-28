#include "RealMain.h"

#include "Engine.h"

int RealLuanch::Run(const wchar_t* Cmd)
{
    REngine::Get()->OnInit();

    while(true)
    {
        REngine::Get()->OnUpdate();
    }
    return 0;
}


int RealMain(const wchar_t* Cmd)
{
    return  RealLuanch().Run(Cmd);
}

