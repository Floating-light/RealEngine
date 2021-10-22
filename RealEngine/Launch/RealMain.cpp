#include "RealMain.h"

#include "GraphicInterface.h"
#include "Engine.h"
#include "Core.h"

// for static library reference 
#include "RHID3D12.h"

int RealMain(const wchar_t* Cmd)
{
    return  RealLuanch().Run(Cmd);
}
int RealLuanch::Run(const wchar_t* Cmd)
{
    RHIInit();

    REngine::Get()->OnInit();

    while(!RCoreGlobal::IsRequireExit())
    {
        REngine::Get()->OnUpdate();
    }

    return 0;
}


