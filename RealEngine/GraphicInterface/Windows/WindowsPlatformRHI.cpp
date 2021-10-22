#include "GraphicInterface.h"
#include "Logging.h"

RGraphicInterface* CreatePlatformRHI()
{
    auto Result = RGraphicInterface::m_Factory.find(L"D3D12RHI");
    
    if(Result != RGraphicInterface::m_Factory.end())
    {
        return Result->second();
    }
    else
    {
        RLOG(ERROR) << "Create platform rhi failed " << std::endl;
        return nullptr;
    }
}
