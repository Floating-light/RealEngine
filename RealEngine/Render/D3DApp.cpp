#include "D3DApp.h"
#include "Helper.h"

#include <iostream>
#include <vector>
#include <filesystem>

D3DApp::D3DApp()
    // : m_clientWidth(width)
    // , m_clientHeight(height)
    // , m_title(title)
    // , m_currentBackBuffer(0)
    // , m_viewport((width - height)/2,0.0f,height, height)
    // // , m_viewport(0.0f,0.0f,width, height)

    // , m_scissorRect(0,0, static_cast<LONG>(width), static_cast<LONG>(height))
    // , m_scissorRect((width - height)/2,0, static_cast<LONG>( height), static_cast<LONG>(height))

{
    m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    m_depthStencilFormat = DXGI_FORMAT_D32_FLOAT;
    m_4xMsaaState = false;
    m_4xMassQuality = 4;
    // m_aspectRatio = static_cast<float>(width ) / static_cast<float>(height);
    m_aspectRatio = 1.7f;

    // m_aspectRatio = 1;
}
void D3DApp::InitializeViewport(void* hHwnd, unsigned int width, unsigned int height, const std::wstring& title)
{
    m_clientWidth = width;
    m_clientHeight = height;
    m_title = title;
    m_viewport = CD3DX12_VIEWPORT(static_cast<float>(width - height)/2,0.0f,static_cast<float>(height), static_cast<float>(height));
    m_aspectRatio = static_cast<float>(width ) / static_cast<float>(height);
    m_scissorRect = CD3DX12_RECT(0,0, static_cast<LONG>(width), static_cast<LONG>(height));
    m_hHwnd = HWND(hHwnd);
}

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
    {
        D3D_FEATURE_LEVEL featureLevels[3] = {D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3} ;
        D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelsInfo;
        featureLevelsInfo.NumFeatureLevels = 3;
        featureLevelsInfo.pFeatureLevelsRequested = featureLevels;
        m_device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelsInfo, sizeof(featureLevelsInfo));
        std::cout <<"Max supported feature : " <<  std::hex << featureLevelsInfo.MaxSupportedFeatureLevel << std::endl;
    }

    // 4X MSAA support query
    {
        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
        msQualityLevels.Format = m_backBufferFormat;
        msQualityLevels.SampleCount = 4;
        msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
        msQualityLevels.NumQualityLevels = 0;
        ThrowIfFailed(m_device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels,sizeof(msQualityLevels)));
        assert(msQualityLevels.NumQualityLevels > 0);
    }
    // Cache descriptor size 
    m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_dsvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    m_cbvUavDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // Create command queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

    // m_CommandQueue->ExecuteCommandLists()

    m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(&m_commandAllocator));

    // nodeMask the index of GPU (For multple gpu system)
    // pInitialState can be nullptr if CommandList will not contain draw command.
    // but we need .
    // m_device->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(),nullptr, IID_PPV_ARGS(&m_commandList));
    // for a command allocator, only can have a commandlist is openning fo record command in the same time.
    
    std::cout << "GPU number : "<< m_device->GetNodeCount() << std::endl;

    // CommandList record command 
    // m_commandList->Close();

    // ID3D12CommandList* ppCommandLists[] = { m_commandList.Get()};
    // m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // the command in queue will be ok, that will be maintaine by Allocator.
    // m_commandList->Reset(m_commandAllocator.Get(),nullptr);

    // dangerous, the command has not be executed by gpu !!!
    // should use fences to determine GPU execution progress!
    // m_commandAllocator->Reset();

    // Create seap chain 
    CreateSwapChain();

    // Create rtv descriptor heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
        rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rtvHeapDesc.NodeMask = 0;
        ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
    }
    
    // Create render target view (descriptor)
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
        for(UINT i = 0; i < SwapChainBufferCount; i++)
        {
            m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
            m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHeapHandle);
            rtvHeapHandle.Offset(1, m_rtvDescriptorSize);
        }
    }
    // Create Depth/Stencil view descriptor heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
        dsvHeapDesc.NumDescriptors = 1;
        dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        dsvHeapDesc.NodeMask = 0;

        ThrowIfFailed(m_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap)));
    }

    // Create depth / stencil buffer and view(descriptor)
    // use CreateCommittedResource()
    {
        // Resource desc 
        D3D12_RESOURCE_DESC desc;
        desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        desc.Alignment = 0;
        desc.Height = m_clientHeight;
        desc.Width = m_clientWidth;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = m_depthStencilFormat;
        desc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
        desc.SampleDesc.Quality = m_4xMsaaState ? m_4xMsaaState - 1 : 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        D3D12_CLEAR_VALUE ClearValue;
        ClearValue.Format = m_depthStencilFormat;
        ClearValue.DepthStencil.Depth = 1.0f;
        ClearValue.DepthStencil.Stencil = 0.0f; 
        CD3DX12_HEAP_PROPERTIES Tempp(D3D12_HEAP_TYPE_DEFAULT);
        // Create resource 
        m_device->CreateCommittedResource(&Tempp, 
                                          D3D12_HEAP_FLAG_NONE,
                                          &desc,
                                          D3D12_RESOURCE_STATE_COMMON,
                                          &ClearValue,IID_PPV_ARGS(&m_depthStencilBuffer));
        // Create view(descriptor)
        // the resource type has been descript when create , so the second paramter can be nullptr, 
        // indicate use the desc when it create. if it is a none type resource , must specify type here.
        m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(),nullptr, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
    }

    LoadAsset();
}

void D3DApp::LoadAsset()
{
    // Create an empty root signature
    {
        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
        rootSignatureDesc.Init(0,nullptr, 0,nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1,&signature, &error));
        ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
    }

    // Create pipeline state, which includes compiling and loading shaders
    {
        ComPtr<ID3DBlob> vertexShader;
        ComPtr<ID3DBlob> pixelShader;
#if defined(DEBUG)
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif
        std::wstring shaderPath = GetShaderPath() + L"MainShader.hlsl";
        ThrowIfFailed(D3DCompileFromFile(shaderPath.c_str(), nullptr, nullptr,
        "VSMain","vs_5_0", compileFlags,0,&vertexShader, nullptr ));
        ThrowIfFailed(D3DCompileFromFile(shaderPath.c_str(), nullptr, nullptr, 
        "PSMain", "ps_5_0",compileFlags, 0, &pixelShader, nullptr ));

        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = 
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
        };

        // Create graphics gipeline state object 
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = {inputElementDescs, _countof(inputElementDescs)};
        psoDesc.pRootSignature = m_rootSignature.Get();
        psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
        psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState.DepthEnable = FALSE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;
        ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
    }

    ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList) ));

    m_commandList->Close();

    // Create the vertex buffer
    {
        // Vertex triangleVertices[] = 
        // {
        //     { { 0.0f, 0.25f*m_aspectRatio , 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        //     { { 0.25f, -0.25f*m_aspectRatio , 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        //     { { -0.25f, -0.25f *m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
        // };
        // Vertex triangleVertices[] = 
        // {
        //     { { 0.0f, 1.0f*m_aspectRatio , 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        //     { { 1.0f, -1.0f*m_aspectRatio , 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        //     { { -1.0f, -1.0f *m_aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
        // };
        Vertex triangleVertices[] = 
        {
            { { -1.0f, -1.0f , 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { -1.0f, 1.0f , 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 1.0f, -1.0f , 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },

            { { 1.0f, 1.0f , 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { 1.0f, -0.8f , 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.8f, 1.0f , 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } }
        };
        const UINT vertexBufferSize = sizeof(triangleVertices);

        CD3DX12_HEAP_PROPERTIES heapPro(D3D12_HEAP_TYPE_UPLOAD);
        auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
        ThrowIfFailed(m_device->CreateCommittedResource(&heapPro, 
            D3D12_HEAP_FLAG_NONE,
            &resDesc, 
            D3D12_RESOURCE_STATE_GENERIC_READ, 
            nullptr, 
            IID_PPV_ARGS(&m_vertexBuffer)));

        // UINT8 * pVertexDataBegin;

        // Copy the triangle data to the vertex buffer
        CD3DX12_RANGE readRange(0,0);
        ThrowIfFailed(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
        memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
        // m_vertexBuffer->Unmap(0, nullptr);

        m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
        m_vertexBufferView.StrideInBytes = sizeof(Vertex);
        m_vertexBufferView.SizeInBytes = vertexBufferSize;
    }

    // Line : 
    {
        Vertex LineVertex[] = 
        {
            { {-0.9f, -0.9f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            { {0.9f, -0.9f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            { {0.9f, 0.9f , 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f}},
            { {-0.9f, 0.9f , 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f}}

        };
        const UINT bufferSize = sizeof(LineVertex);
        auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
        ThrowIfFailed(m_device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_LineBuffer)
        ));
        auto range = CD3DX12_RANGE(0,0);
        ThrowIfFailed(m_LineBuffer->Map(0,&range,reinterpret_cast<void**>(&pLineDataBegin)));
        memcpy(pLineDataBegin, LineVertex, bufferSize);
        m_LineBuffer->Unmap(0, nullptr);

        m_lineVertexBufferView.BufferLocation = m_LineBuffer->GetGPUVirtualAddress();
        m_lineVertexBufferView.SizeInBytes = bufferSize;
        m_lineVertexBufferView.StrideInBytes = sizeof(Vertex);
    }


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

void D3DApp::OnUpdate(double DeltaTime)
{
    static double Data = 0.0f;
    Data += DeltaTime;

    // Vertex* V = reinterpret_cast<Vertex*>(pVertexDataBegin);
    // V[0].position.y = 1.0f *  abs(sinf(Data)) ;
    // V[1].position.x = 1.0f *  abs(sinf(Data) );
    // V[1].position.y = -1.0f * abs(sinf(Data));
    // V[2].position.x = -1.0f * abs(sinf(Data));
    // V[2].position.y = -1.0f * abs(sinf(Data));


}
void D3DApp::OnRender()
{
    PopulateCommandList();

    ID3D12CommandList* ppCommandLists[] = { m_commandList.Get()};
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    ThrowIfFailed(m_swapChain->Present(0, 0));

    WaitForPreviousFrame();
}
// https://www.braynzarsoft.net/viewtutorial/q16390-04-direct3d-12-drawing
void D3DApp::PopulateCommandList()
{
    ThrowIfFailed(m_commandAllocator->Reset());

    ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()));

    m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    m_commandList->RSSetViewports(1, &m_viewport);
    m_commandList->RSSetScissorRects(1, &m_scissorRect);
    auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_currentBackBuffer].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_commandList->ResourceBarrier(1, &resourceBarrier);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),m_currentBackBuffer, m_rtvDescriptorSize);
    m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    const float clearColor[] = {0.0f, 0.2f, 0.4f, 1.0f};
    m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_commandList->IASetVertexBuffers(0,1,&m_vertexBufferView);
    m_commandList->DrawInstanced(6, 1,0,0);

    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
    m_commandList->IASetVertexBuffers(0,1,&m_lineVertexBufferView);
    m_commandList->DrawInstanced(4, 1,0,0);

    auto rt2PresentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_currentBackBuffer].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_commandList->ResourceBarrier(1, &rt2PresentBarrier);

    ThrowIfFailed(m_commandList->Close());
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

    adapterList.clear();

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

    m_currentBackBuffer = m_swapChain->GetCurrentBackBufferIndex();

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
    desc.BufferDesc.Width = m_clientWidth;
    desc.BufferDesc.Height = m_clientHeight;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.Format = m_backBufferFormat;

    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.SampleDesc.Count = m_4xMsaaState ? 4: 1;
    desc.SampleDesc.Quality = m_4xMsaaState ? m_4xMassQuality - 1 : 0;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = SwapChainBufferCount;
    desc.OutputWindow = AppWindow::Get().GetHwnd();

    desc.Windowed = true;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    ComPtr<IDXGISwapChain> SwapChainToCreate;
    // need a command queue to refersh swapchain
    ThrowIfFailed(m_factory->CreateSwapChain(m_commandQueue.Get(), &desc, &SwapChainToCreate));
    ThrowIfFailed(SwapChainToCreate.As(&m_swapChain));

    ThrowIfFailed(m_factory->MakeWindowAssociation(AppWindow::Get().GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

    m_currentBackBuffer = m_swapChain->GetCurrentBackBufferIndex();
}

std::wstring D3DApp::GetShaderPath() const 
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::wcout << "current path : " << currentPath.generic_wstring() << std::endl;
    return currentPath.generic_wstring() + L"/../../../RealEngine/Render/";
    // return  L"/sdf";
}

