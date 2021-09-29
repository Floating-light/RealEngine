#include "RealMain.h"

#include "Engine.h"
#include "Core.h"

int RealMain(const wchar_t* Cmd)
{
    return  RealLuanch().Run(Cmd);
}
int RealLuanch::Run(const wchar_t* Cmd)
{
    REngine::Get()->OnInit();

    while(!RCoreGlobal::IsRequireExit())
    {
        REngine::Get()->OnUpdate();
    }

    return 0;
}


