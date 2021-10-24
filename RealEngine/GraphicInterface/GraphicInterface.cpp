#include "GraphicInterface.h"
#include "Logging.h"

RGraphicInterface* GGraphicInterface = nullptr;
std::unordered_map<std::wstring, std::function<RGraphicInterface* ()>>  RGraphicInterface::m_Factory = {};
// #pragma comment(linker, "/include:IMPLEMENT_MODULE_D3D12RHI")

void RHIInit()
{
    if(GGraphicInterface = CreatePlatformRHI())
    {
        GGraphicInterface->InitRHI();
    }
    else
    {
        RLOG(FATAL) << "Create graphic interface failed " << std::endl;
    }
}
void RHIExit()
{
    
}