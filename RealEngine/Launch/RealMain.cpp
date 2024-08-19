#include "RealMain.h"

#include <chrono>

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
    auto timeStart = std::chrono::high_resolution_clock::now();
    while(!RCoreGlobal::IsRequireExit())
    {
        const auto timeEnd = std::chrono::high_resolution_clock::now();
        const float deltaSeconds = std::chrono::duration<float>(timeEnd - timeStart).count();
        timeStart = timeEnd;
        REngine::Get()->OnUpdate(deltaSeconds);
    }
    REngine::Get()->Destoryed(); 
    RHIExit(); 
    return 0;
}


