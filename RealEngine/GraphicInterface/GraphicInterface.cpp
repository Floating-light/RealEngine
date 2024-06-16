#include "GraphicInterface.h"
#include "Logging.h"
#include "CommandContext.h"
#include "Adapter.h"
#include "CommandListManager.h"

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
        RLOG(Fatal, "Create graphic interface failed ");
        // 暂时全部实现在这个模块
    }
}
void RHIExit()
{
    GGraphicInterface->DeInitRHI();
    delete GGraphicInterface;

    Microsoft::WRL::ComPtr<IDXGIDebug1> debug1 = nullptr;
    ASSERT(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug1))); 
    auto hr = debug1->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL); 
    if (FAILED(hr)) {
        // 处理错误
    }
}

void RGraphicInterface::InitRHI()
{
    using Microsoft::WRL::ComPtr;

    uint32_t useDebugLayers = 1;
    if (useDebugLayers)
    {
        ComPtr<ID3D12Debug> debugInterface;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)))) 
        {
            debugInterface->EnableDebugLayer();
            uint32_t useGPUBasedValidation = 1;
            if (useGPUBasedValidation)
            {
                ComPtr<ID3D12Debug1> debugInterface1;
                if (SUCCEEDED(debugInterface->QueryInterface(IID_PPV_ARGS(&debugInterface1))))
                {
                    debugInterface1->SetEnableGPUBasedValidation(true);
                }
            }
        }
    }

    UINT dxgiFactoryFlags = 0;
    //ComPtr<IDXGIFactory4> dxgiFactory4;
    if(FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_Factory4))))
    {
        RLOG(Fatal, "CreateDXGIFactory2 failed ");
        return ;
    }
    // ComPtr<IDXGIAdapter> Adapter;
    TRefCountPtr<IDXGIAdapter> MyAdapter;
    for(int i = 0; DXGI_ERROR_NOT_FOUND != m_Factory4->EnumAdapters(i, MyAdapter.GetInitReference()); ++i)
    {
        DXGI_ADAPTER_DESC desc;
        MyAdapter->GetDesc(&desc);
        RLOG(Info, "Description: {}, VendorId: {}, DeviceId: {}, SubSysId: {}, Revision: {}, DedicatedVideoMemory: {}, DedicatedSystemMomory: {}, SharedSystemMemory: {}, AdapterLuid lowPart {}, HighPart: {}",
            RUtility::WideStringToString(desc.Description), desc.VendorId, desc.DeviceId, desc.SubSysId, desc.Revision,desc.DedicatedVideoMemory,
             desc.DedicatedSystemMemory, desc.SharedSystemMemory, desc.AdapterLuid.LowPart, desc.AdapterLuid.HighPart);

        if(SUCCEEDED(D3D12CreateDevice(MyAdapter.GetReference(),D3D_FEATURE_LEVEL_11_0,IID_PPV_ARGS(&m_Device))))
        {
            break;
        }
    }
    assert(m_Device != nullptr); 
    if(MyAdapter)
    {
        mAdapter = std::shared_ptr<RAdapter>(new RAdapter(MyAdapter));
    }
    ContextManager = std::make_unique<RCommandContextManger>();
    CommandListManager = std::make_unique<RCommandListManager>();
    CommandListManager->Create(m_Device.Get()); 
}

void RGraphicInterface::DeInitRHI()
{
    CommandListManager->IdleGPU();

    CommandListManager->ShutDown();
    ContextManager->DestroyAllContexts();
}

void RGraphicInterface::InitilizeViewport(HWND WindowHandle, uint32_t Width, uint32_t Height)
{
    assert(!Viewport);
    Viewport = std::make_unique<RGraphicViewport>();
    Viewport->SetViewportSize(Width, Height);
    Viewport->Initialize(m_Device.Get(), m_Factory4.Get(), CommandListManager->GetGraphicsQueue().GetCommandQueue(), WindowHandle);
}

void RGraphicInterface::Present()
{
    Viewport->Present();
}

TRefCountPtr<RRHIBuffer> RGraphicInterface::CreateBuffer(const void *Data, uint32_t Size, uint32_t Stride, std::string_view DebugName)
{
    // TODO:
    //return mCommandContext->CreateBuffer(Data, Size, Stride,DebugName);
    return nullptr;
}

RCommandContext* RGraphicInterface::BeginCommandContext(const std::string& ID) 
{
    RCommandContext* NewContext = ContextManager->AllocateContext(D3D12_COMMAND_LIST_TYPE_DIRECT);   
    NewContext->SetID(ID);
    return NewContext; 
}
