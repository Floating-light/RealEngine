#include "RHID3D12.h"
#include "Logging.h"
#include "D3D12Adapter.h"

static void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter)
{

}
void RHID3D12::InitRHI() 
{
    UINT dxgiFactoryFlags = 0;
    ComPtr<IDXGIFactory4> dxgiFactory4;
    if(FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory4))))
    {
        RLOG(FATAL) << "CreateDXGIFactory2 failed " << std::endl;
        return ;
    }


}
