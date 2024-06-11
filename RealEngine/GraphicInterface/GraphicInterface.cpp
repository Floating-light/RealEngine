#include "GraphicInterface.h"
#include "Logging.h"
#include "CommandContext.h"
#include "Adapter.h"

RGraphicInterface* GGraphicInterface = nullptr;

void RHIInit()
{
    GGraphicInterface = new RGraphicInterface();
    if(GGraphicInterface /*= CreatePlatformRHI() */)
    {
        GGraphicInterface->InitRHI();
    }
    else
    {
        // RLOG(Fatal, "Create graphic interface failed ");
        // 暂时全部实现在这个模块
    }
}
void RHIExit()
{
    
}

void RGraphicInterface::InitRHI()
{
    using Microsoft::WRL::ComPtr;

    UINT dxgiFactoryFlags = 0;
    ComPtr<IDXGIFactory4> dxgiFactory4;
    if(FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory4))))
    {
        RLOG(Fatal, "CreateDXGIFactory2 failed ");
        return ;
    }
    // ComPtr<IDXGIAdapter> Adapter;
    TRefCountPtr<IDXGIAdapter> MyAdapter;
    for(int i = 0; DXGI_ERROR_NOT_FOUND != dxgiFactory4->EnumAdapters(i, MyAdapter.GetInitReference()); ++i)
    {
        DXGI_ADAPTER_DESC desc;
        MyAdapter->GetDesc(&desc);
        RLOG(Info, "Description: {}, VendorId: {}, DeviceId: {}, SubSysId: {}, Revision: {}, DedicatedVideoMemory: {}, DedicatedSystemMomory: {}, SharedSystemMemory: {}, AdapterLuid lowPart {}, HighPart: {}",
            RUtility::WideStringToString(desc.Description), desc.VendorId, desc.DeviceId, desc.SubSysId, desc.Revision,desc.DedicatedVideoMemory,
             desc.DedicatedSystemMemory, desc.SharedSystemMemory, desc.AdapterLuid.LowPart, desc.AdapterLuid.HighPart);

        if(SUCCEEDED(D3D12CreateDevice(MyAdapter.GetReference(),D3D_FEATURE_LEVEL_11_0,_uuidof(ID3D12Device), nullptr)))
        {
            break;
        }
    }
    if(MyAdapter)
    {
        mAdapter = std::shared_ptr<RAdapter>(new RAdapter(MyAdapter));
    }
    mCommandContext = nullptr;
}

TRefCountPtr<RRHIBuffer> RGraphicInterface::CreateBuffer(const void *Data, uint32_t Size, uint32_t Stride, std::string_view DebugName)
{
    return mCommandContext->CreateBuffer(Data, Size, Stride,DebugName);
}
