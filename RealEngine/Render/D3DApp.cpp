#include "D3DApp.h"
#include "Helper.h"
#include <iostream>
#include <vector>
void D3DApp::Setup()
{
    UINT dxgiFactoryFlags = 0;
    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_factory)));

    ComPtr<IDXGIAdapter> warpAdapter;

    ThrowIfFailed(m_factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
    ThrowIfFailed(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));

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

    ComPtr<ID3D12GraphicsCommandList> CommandList;
    // nodeMask the index of GPU (For multple gpu system)
    // pInitialState can be nullptr if CommandList will not contain draw command.
    m_device->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(),nullptr, IID_PPV_ARGS(&CommandList));
    // for a command allocator, only can have a commandlist is openning fo record command in the same time.
    
    // CommandList record command 
    
    CommandList->Close();

    ID3D12CommandList* ppCommandLists[] = { CommandList.Get()};
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // the command in queue will be ok, that will be maintaine by Allocator.
    CommandList->Reset(m_commandAllocator.Get(),nullptr);

    // dangerous, the command has not be execute by gpu !!!
    // should use fences to determine GPU execution progress!
    // m_commandAllocator->Reset();
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

