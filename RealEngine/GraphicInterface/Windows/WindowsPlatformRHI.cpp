#include "GraphicInterface.h"
#include "Logging.h"

RGraphicInterface* CreatePlatformRHI()
{
    GraphicModuleBase* D3D12Module = RModuleManager::Get().GetModule<GraphicModuleBase>(L"D3D12RHI");
    
    if(D3D12Module)
    {
        return D3D12Module->CreateRHI();
    }
    else
    {
        RLOG(ERROR) << "Create platform rhi failed " << std::endl;
        return nullptr;
    }
}
