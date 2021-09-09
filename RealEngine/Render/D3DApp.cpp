#include "D3DApp.h"
#include "Helper.h"
#include <iostream>
#include <vector>

void D3DApp::Setup()
{
#if defined(DEBUG) || defined(_DEBUG)
    {
        ComPtr<ID3D12Debug> debugController;
        ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
        debugController->EnableDebugLayer();
    }
#endif
    UINT dxgiFactoryFlags = 0;
    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_factory)));

    // Hardware adapter
    ComPtr<IDXGIAdapter1> adapter;
    GetHardwareAdapter(m_factory.Get(), &adapter);

    if(FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device))))
    {
        // https://docs.microsoft.com/en-us/windows/win32/direct3darticles/directx-warp
        ThrowIfFailed(m_factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter)));

        ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
    }
   
    // feature to query if support
    D3D_FEATURE_LEVEL featureLevels[3] = {D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3} ;
    D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelsInfo;
    featureLevelsInfo.NumFeatureLevels = 3;
    featureLevelsInfo.pFeatureLevelsRequested = featureLevels;

    m_device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelsInfo, sizeof(featureLevelsInfo));
    std::cout <<"Max supported feature : " <<  std::hex << featureLevelsInfo.MaxSupportedFeatureLevel << std::endl;

    // Create command queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

    // m_CommandQueue->ExecuteCommandLists()

    m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(&m_commandAllocator));

    // nodeMask the index of GPU (For multple gpu system)
    // pInitialState can be nullptr if CommandList will not contain draw command.
    m_device->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(),nullptr, IID_PPV_ARGS(&m_commandList));
    // for a command allocator, only can have a commandlist is openning fo record command in the same time.
    
    std::cout << "GPU number : "<< m_device->GetNodeCount() << std::endl;

    // CommandList record command 
    m_commandList->Close();

    ID3D12CommandList* ppCommandLists[] = { m_commandList.Get()};
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // the command in queue will be ok, that will be maintaine by Allocator.
    m_commandList->Reset(m_commandAllocator.Get(),nullptr);

    // dangerous, the command has not be executed by gpu !!!
    // should use fences to determine GPU execution progress!
    // m_commandAllocator->Reset();

    // Create seap chain 
    CreateSwapChain();

    // Create rtv descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
    rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDesc.NodeMask = 0;

    ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_swapChain)));
    m_rtvDescHeapIncSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    
    // Create render target view (descriptor)
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for(UINT i = 0; i < SwapChainBufferCount; i++)
    {
        m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
        m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHeapHandle);
        rtvHeapHandle.Offset(1, m_rtvDescHeapIncSize);
    }
    // Create Depth/Stencil view descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
    rtvHeapDesc.NumDescriptors = 1;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDesc.NodeMask = 0;

    ThrowIfFailed(m_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap)));
    m_dsvDescHeapIncSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

}

void D3DApp::LoadAsset()
{
    // use commandlist to load asset 

    {
        ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
        m_fenceValue = 1; // next usable fence value 

        // Create envent to handle fence commplete
        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if(m_fenceEvent == nullptr)
        {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }
        
        // wait gpu to commplete command 
        WaitForPreviousFrame();
    }
}

void D3DApp::LogAdapters() 
{
    UINT i = 0;
    ComPtr<IDXGIAdapter> adapter  = nullptr;
    std::vector<ComPtr<IDXGIAdapter>> adapterList;
    while(m_factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc(&desc);

        std::wstring text = L"***Adapter: ";
        text += desc.Description ;
        text += L"\n";

        OutputDebugString(text.c_str());

        adapterList.push_back(adapter);

        LogAdapterOutputs(adapterList[i], desc.Description);
        ++i;
    }

    adapterList.empty();

}

void D3DApp::LogAdapterOutputs(ComPtr<IDXGIAdapter> adapter, const std::wstring& desctiption)
{
    UINT i = 0;
    ComPtr<IDXGIOutput> output = nullptr;

    std::wstring title = desctiption +L":\n"; 
    OutputDebugString(title.c_str());
    while(adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_OUTPUT_DESC desc;
        output->GetDesc(&desc);

        std::wstring text = L"---> Output: ";
        text += desc.DeviceName;
        text += L"\n";
        OutputDebugString(text.c_str());

        LogOutputDisplayModels(output, DXGI_FORMAT_B8G8R8A8_UNORM);
        
        ++i;
    }
}

void D3DApp::LogOutputDisplayModels(ComPtr<IDXGIOutput> output, DXGI_FORMAT format)
{
    UINT flag = 0, count = 0;

    // pass nullptr to get display mode number only
    output->GetDisplayModeList(format,flag,&count,nullptr);

    std::vector<DXGI_MODE_DESC> modelList(count);
    output->GetDisplayModeList(format, flag, &count, &modelList[0]);

    for(const auto& item : modelList)
    {
        UINT n = item.RefreshRate.Numerator;
        UINT d = item.RefreshRate.Denominator;

        std::wstring text = L"Width = " + std::to_wstring(item.Width) + L" " + 
        L"Heigh = " + std::to_wstring(item.Height) + L" " + 
        L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) + L"\n";

        ::OutputDebugString(text.c_str()); 
    }

}

void D3DApp::WaitForPreviousFrame()
{
    UINT64 CurrentFence = m_fenceValue;
    m_commandQueue->Signal(m_fence.Get(), CurrentFence);
    m_fenceValue++;

    if(m_fence->GetCompletedValue() < CurrentFence)
    {
        m_fence->SetEventOnCompletion(CurrentFence, m_fenceEvent);
        WaitForSingleObject(m_fenceEvent, INFINITE);  // return untile gpu set m_fence to CurrentFence value. 
    }

}

void D3DApp::GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter )
{
    *ppAdapter = nullptr;

    ComPtr<IDXGIAdapter1> adapter;

    ComPtr<IDXGIFactory6> factory6;
    if(SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        for(UINT adapterIndex = 0; 
        DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
            adapterIndex,
            requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
            IID_PPV_ARGS(&adapter));
            ++adapterIndex)
            {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);
                if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    // not software rasterization, use factory->EnumWarpAdapter
                    continue ;
                }
                // check to see whether the adapter supports Direct3D12
                if(SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                {
                    break ;
                }
            }
    }
    else // factory1
    {
        for(UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                continue;
            }
            if(SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }
    *ppAdapter = adapter.Detach();
}

void D3DApp::CreateSwapChain()
{
    m_swapChain.Reset();

    DXGI_SWAP_CHAIN_DESC desc;
    desc.BufferDesc.Width = 720;
    desc.BufferDesc.Height = 1280;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferCount = 2;
    desc.OutputWindow = m_hMainWnd;

    desc.Windowed = true;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // need a command queue to refersh swapchain
    ThrowIfFailed(m_factory->CreateSwapChain(m_commandQueue.Get(), &desc, m_swapChain.GetAddressOf()));
}
