#include "RHID3D12.h"
#include "Core.h"
#include "D3D12Adapter.h"
// ZMACRO_MSVCStaticLib_cpp(RHID3D12)
RDEFINE_MOUDLE(RHID3D12Module,D3D12RHI)

// 

static void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter)
{

}
void RHID3D12::InitRHI() 
{
    UINT dxgiFactoryFlags = 0;
    ComPtr<IDXGIFactory4> dxgiFactory4;
    if(FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory4))))
    {
        RLOG(Fatal, "CreateDXGIFactory2 failed ");
        return ;
    }
    ComPtr<IDXGIAdapter> Adapter;
    for(int i = 0; DXGI_ERROR_NOT_FOUND != dxgiFactory4->EnumAdapters(i, &Adapter); ++i)
    {
        DXGI_ADAPTER_DESC desc;
        Adapter->GetDesc(&desc);
        RLOG(Info, "Description: {}, VendorId: {}, DeviceId: {}, SubSysId: {}, Revision: {}, DedicatedVideoMemory: {}, DedicatedSystemMomory: {}, SharedSystemMemory: {}, AdapterLuid lowPart {}, HighPart: {}",
            RUtility::WideStringToString(desc.Description), desc.VendorId, desc.DeviceId, desc.SubSysId, desc.Revision,desc.DedicatedVideoMemory,
             desc.DedicatedSystemMemory, desc.SharedSystemMemory, desc.AdapterLuid.LowPart, desc.AdapterLuid.HighPart);

        if(SUCCEEDED(D3D12CreateDevice(Adapter.Get(),D3D_FEATURE_LEVEL_11_0,_uuidof(ID3D12Device), nullptr)))
        {
            break;
        }
    }
    if(Adapter)
    {
        m_adapter = std::shared_ptr<RD3D12Adapter>(new RD3D12Adapter(Adapter));
    }
}


void RHID3D12Module::InitializeModule()
{

}
void RHID3D12Module::DeInitializeModule()
{

} 
RGraphicInterface* RHID3D12Module::CreateRHI() 
{
    return new RHID3D12;
}