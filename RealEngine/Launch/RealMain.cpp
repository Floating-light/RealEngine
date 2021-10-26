#include "RealMain.h"

#include "GraphicInterface.h"
#include "Engine.h"
#include "Core.h"
#include "stdio.h"
// for static library reference 
// #include "RHID3D12.h"
int RealMain(const wchar_t* Cmd)
{
    return  RealLuanch().Run(Cmd);
}
int RealLuanch::Run(const wchar_t* Cmd)
{
    std::wstring strCmd = Cmd;
    std::wstring rPath(strCmd.begin() + 1, strCmd.end()-2);
    std::size_t lastSlashPos = rPath.find_last_of('\\');
    if(lastSlashPos != std::string::npos)
    {
        ExecutableDir = rPath.substr(0,lastSlashPos);
        InitLogger(ExecutableDir);
    }
    RHIInit();

    REngine::Get()->OnInit();

    while(!RCoreGlobal::IsRequireExit())
    {
        REngine::Get()->OnUpdate();
    }
    return 0;
}


