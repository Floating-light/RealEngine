#include "GraphicInterface.h"
#include "Logging.h"

RGraphicInterface* CreatePlatformRHI()
{
    GraphicModuleBase* D3D12Module = RModuleManager::Get().GetModule<GraphicModuleBase>("D3D12RHI");
    
    if(D3D12Module)
    {
        return D3D12Module->CreateRHI();
    }
    else
    {
        RLOG(Error, "Create platform rhi failed ");
        return nullptr;
    }
}
