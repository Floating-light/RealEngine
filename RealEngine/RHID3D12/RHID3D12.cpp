#include "RHID3D12.h"
#include "Logging.h"
#include "D3D12Adapter.h"
ZMACRO_MSVCStaticLib_cpp(RHID3D12)
static RHIInterfaceRegistrant<RHID3D12> D3D12RHIRegistrant(L"D3D12RHI");
// 

static void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter)
{

}
void RHID3D12::InitRHI() 
{
    D3D12RHIRegistrant.Initlialize();;
    UINT dxgiFactoryFlags = 0;
    ComPtr<IDXGIFactory4> dxgiFactory4;
    if(FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory4))))
    {
        RLOG(FATAL) << "CreateDXGIFactory2 failed " << std::endl;
        return ;
    }
    ComPtr<IDXGIAdapter> Adapter;
    for(int i = 0; DXGI_ERROR_NOT_FOUND != dxgiFactory4->EnumAdapters(i, &Adapter); ++i)
    {
        DXGI_ADAPTER_DESC desc;
        Adapter->GetDesc(&desc);
        RLOG(INFO) << "Description : " << desc.Description 
                   << ", VendorId : " << desc.VendorId
                   << ", DeviceId : " << desc.DeviceId
                   << ", SubSysId : " << desc.SubSysId
                   << ", Revision : " << desc.Revision
                   << ", DedicatedVideoMemory : " << desc.DedicatedVideoMemory
                   << ", DedicatedSystemMomory : " << desc.DedicatedSystemMemory
                   << ", SharedSystemMemory : " << desc.SharedSystemMemory
                   << ", AdapterLuid : LowPart: " << desc.AdapterLuid.LowPart << ", HighPart: " << desc.AdapterLuid.HighPart << std::endl;
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
