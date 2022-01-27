#include "Device.h"
#include "Helper.h"
#include "Core.h"

static void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter )
{
    *ppAdapter = nullptr;

    ComPtr<IDXGIAdapter1> TempAdapter;

    ComPtr<IDXGIFactory6> factory6;
    if(SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        for(UINT adapterIndex = 0; 
        DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
            adapterIndex,
            requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
            IID_PPV_ARGS(&TempAdapter));
            ++adapterIndex)
            {
                DXGI_ADAPTER_DESC1 desc;
                TempAdapter->GetDesc1(&desc);
                if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    // not software rasterization, use factory->EnumWarpAdapter
                    continue ;
                }
                // check to see whether the adapter supports Direct3D12
                // do not actually created  
                if(SUCCEEDED(D3D12CreateDevice(TempAdapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                {
                    break ;
                }
            }
    }
    else // factory1
    {
        for(UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &TempAdapter); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            TempAdapter->GetDesc1(&desc);

            if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                continue;
            }
            if(SUCCEEDED(D3D12CreateDevice(TempAdapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }
    *ppAdapter = TempAdapter.Detach();
}

RDevice::RDevice()
{
#if defined(DEBUG) || defined(_DEBUG)
    {
        ComPtr<ID3D12Debug> debugController;
        ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
        debugController->EnableDebugLayer();
    }
#endif
    // DXGI_CREATE_FACTORY_DEBUG or 0;
    UINT dxgiFactoryFlags = 0;
    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mFactory)));
    CHECK(mFactory);

    GetHardwareAdapter(mFactory.Get(), &mAdapter, true);
    CHECK(mAdapter);

    if(FAILED(D3D12CreateDevice(mAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice))))
    {
        // do not have hardware divice, create a software warpadapter
        ThrowIfFailed(mFactory->EnumWarpAdapter(IID_PPV_ARGS(&mAdapter)));
        ThrowIfFailed(D3D12CreateDevice(mFactory.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice)));
    }
    CHECK(mDevice);
}
