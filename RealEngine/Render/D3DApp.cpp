#include "D3DApp.h"
#include "Helper.h"
#include "GraphicInterface.h"
#include "CommandContext.h"
#include "CommandListManager.h"
#include "GraphicViewport.h"
#include "RHIUploadBuffer.h"
#include "RHIBuffer.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "PrimitiveInfo.h"
#include "ModelData.h"

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
    m_backBufferFormat = DXGI_FORMAT_R10G10B10A2_UNORM; 
    m_depthStencilFormat = DXGI_FORMAT_D32_FLOAT;
    m_4xMsaaState = false;
    m_4xMassQuality = 4;
    // m_aspectRatio = static_cast<float>(width ) / static_cast<float>(height);
    m_aspectRatio = 1.7f;

    // m_aspectRatio = 1;
}
D3DApp::~D3DApp()
{

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
    // 1. 是否开启Debug
#if defined(DEBUG) || defined(_DEBUG)
    {
        ComPtr<ID3D12Debug> debugController;
        ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
        debugController->EnableDebugLayer();
    }
#endif
    // 2. 创建Factory 
    UINT dxgiFactoryFlags = 0;
    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_factory)));

    // 3. Hardware adapter
    ComPtr<IDXGIAdapter1> adapter;
    GetHardwareAdapter(m_factory.Get(), &adapter);

    // 4. Device
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

    // 4. Create command queue
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

    GGraphicInterface->InitilizeViewport(m_hHwnd, GetWidth(), GetHeight());
    
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

    // Create constant buffer view heap, 
    {
        D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
        cbvHeapDesc.NumDescriptors = 1;
        cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // diff with rt and depth/stencil buffer descriptor heaps .these descriptor will be accessed by shader programs .
        cbvHeapDesc.NodeMask = 0;

        m_device->CreateDescriptorHeap(&cbvHeapDesc,IID_PPV_ARGS(&m_cbvHeap));
    }

    LoadAsset();
}
std::string NewGetShaderPath() 
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::wcout << "current path : " << currentPath.generic_wstring() << std::endl;
    return currentPath.string() + "/../../../RealEngine/Render/";
    // return  L"/sdf";
}
void D3DApp::LoadAsset()
{
    // Create an empty root signature
    {
        CD3DX12_ROOT_PARAMETER slotRootParameter[1] = {};

        //CD3DX12_DESCRIPTOR_RANGE cbvTable;
        //cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); // b0

        //slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);
        slotRootParameter[0].InitAsConstantBufferView(0,0, D3D12_SHADER_VISIBILITY_ALL);

        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
        //rootSignatureDesc.Init(1,nullptr, 0,nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
         rootSignatureDesc.Init(1, slotRootParameter,0,nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT); 
        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1,&signature, &error));
        ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
        m_rootSignature->SetName(L"D3DApp_RootSig");

        std::shared_ptr< RRootSignature> NewSignature = std::shared_ptr<RRootSignature>(new RRootSignature()); 
        NewSignature->AddAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);
        NewSignature->AddAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);
        NewSignature->AddAsConstantBuffer(1, D3D12_SHADER_VISIBILITY_ALL);
        NewSignature->Finalize("MyNewRootSignature", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        m_NewPSO = std::shared_ptr<RGraphicPSO>(new RGraphicPSO("MyNewPSO"));
        D3D12_INPUT_ELEMENT_DESC Desces[] = 
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
        };
        m_NewPSO->SetInputLayout(3, Desces);
        m_NewPSO->SetShader(NewGetShaderPath() + "BaseVS.hlsl", NewGetShaderPath() + "BasePS.hlsl");
        m_NewPSO->SetRootSignature(NewSignature);
        D3D12_BLEND_DESC alphaBlend = {};
        alphaBlend.IndependentBlendEnable = false;
        alphaBlend.AlphaToCoverageEnable = false;
        alphaBlend.RenderTarget[0].BlendEnable = true;
        alphaBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
        alphaBlend.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
        alphaBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD; 
        alphaBlend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE; 
        alphaBlend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA; 
        alphaBlend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD; 
        alphaBlend.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; 
        alphaBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;

        m_NewPSO->SetBlendState(alphaBlend);

        D3D12_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
        rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
        rasterizerDesc.FrontCounterClockwise = TRUE;
        rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        rasterizerDesc.DepthClipEnable = TRUE; 
        rasterizerDesc.MultisampleEnable = FALSE; 
        rasterizerDesc.AntialiasedLineEnable = FALSE;
        rasterizerDesc.ForcedSampleCount = 0;
        rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; 

        m_NewPSO->SetRasterizer(rasterizerDesc);

        D3D12_DEPTH_STENCIL_DESC DepthState = {};
        DepthState.DepthEnable = false;
        DepthState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        DepthState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
        DepthState.StencilEnable = false;
        DepthState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
        DepthState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
        DepthState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        DepthState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        DepthState.FrontFace.StencilFailOp= D3D12_STENCIL_OP_KEEP;
        DepthState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP; 
        DepthState.BackFace = DepthState.FrontFace;

        m_NewPSO->SetDepthStencil(DepthState);
        m_NewPSO->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
        m_NewPSO->SetSampleMask((std::numeric_limits<uint32_t>::max)());
        m_NewPSO->SetRenderTargetFormats(1, &m_backBufferFormat, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN);
        m_NewPSO->Finalize(); 
    }

    // Create pipeline state, which includes compiling and loading shaders
    {
        ComPtr<ID3DBlob> vertexShader;
        ComPtr<ID3DBlob> pixelShader;
        ComPtr<ID3DBlob> errorMsg = nullptr;
#if defined(DEBUG)
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif
        std::wstring shaderPath = GetShaderPath() + L"MainShader.hlsl";
        if (FAILED(D3DCompileFromFile(shaderPath.c_str(), /*Defines*/nullptr, /*Inlcude*/nullptr,
            "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, /*Error*/&errorMsg)))
        {
            std::string errMsg((char* )errorMsg->GetBufferPointer());
            ThrowIfFailed(0);
        }
        if (errorMsg)
        {
            std::string errMsg((char*)errorMsg->GetBufferPointer());
        }

        if (FAILED(D3DCompileFromFile(shaderPath.c_str(), nullptr, nullptr,
            "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, &errorMsg)))
        {
            std::string errMsg((char*)errorMsg->GetBufferPointer());
            ThrowIfFailed(0); 
        }

        if (errorMsg)
        {
            std::string errMsg2((char*)errorMsg->GetBufferPointer());
        }

        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = 
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
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
        psoDesc.RTVFormats[0] = m_backBufferFormat; 
        psoDesc.SampleDesc.Count = 1;
        ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
        m_pipelineState->SetName(L"D3DApp_PSO");
    }

    ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList) ));

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
            { { -1.0f, -1.0f , -1.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { -1.0f, 1.0f , -1.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 1.0f, -1.0f , -1.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },

            { { 1.0f, 1.0f , -1.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { 1.0f, -0.8f , -1.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.8f, 1.0f , -1.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } }
        };
        const UINT vertexBufferSize = sizeof(triangleVertices);
        m_NewUploadVertexBuffer = std::make_unique<RRHIUploadBuffer>(); 
        m_NewUploadVertexBuffer->Create("D3DApp_vertices", vertexBufferSize);
        memcpy(m_NewUploadVertexBuffer->Map(), triangleVertices, vertexBufferSize);
        m_NewUploadVertexBuffer->Unmap();
        
        m_NewVertexBuffer = std::make_unique<RRHIBufferByteAddress>();
        //m_NewVertexBuffer->Create("D3DApp_VDefaultBuffer", vertexBufferSize / sizeof(Vertex), sizeof(Vertex),*m_NewUploadVertexBuffer);
        m_NewUploadVertexBuffer.reset();
        //m_NewVertexBuffer->Destroy();

        m_NewVertexBuffer->Create("D3DApp_InitalData", vertexBufferSize / sizeof(Vertex), sizeof(Vertex), triangleVertices);
        m_vertexBufferView.BufferLocation = m_NewVertexBuffer->GetGPUVirtualAddress(); 
        m_vertexBufferView.StrideInBytes = sizeof(Vertex); // size per element in buffer 
        m_vertexBufferView.SizeInBytes = vertexBufferSize; // total size of buffer 
    }

    // Line vertex buffer : 
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

    ComPtr<ID3D12Resource> UploadLineIndicesRes;
    // Index buffer 
    {
        std::uint16_t LineIndices[] = {0,1,2,3,0};

        // 会调用 Cmd 来Copy buffer , 所以必须执行 commandList.
        // 且
        m_lineIndicesRes = D3DUtil::CreateDefaultBuffer(m_device.Get(),
                                                        m_commandList.Get(),
                                                        LineIndices,
                                                        sizeof(LineIndices),
                                                        UploadLineIndicesRes );

        m_lineIndexbufferView.BufferLocation = m_lineIndicesRes->GetGPUVirtualAddress();
        m_lineIndexbufferView.SizeInBytes = sizeof(LineIndices);
        m_lineIndexbufferView.Format = DXGI_FORMAT_R16_UINT;
    }

    m_commandList->Close();
    
    ID3D12CommandList* CommandLists[] = {m_commandList.Get()};
    m_commandQueue->ExecuteCommandLists(_countof(CommandLists), CommandLists);

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
uint64_t D3DApp::PopulateCommandListNew(const RViewInfo& View)
{
    const std::vector<std::shared_ptr<RPrimitiveObject>>& InPrims = View.GetPrimitives();

    ObjectConstants GlobalConstants;
    GlobalConstants.ViewProjMatrix = View.GetViewProjectionMatrix();
    //GlobalConstants.ViewProjMatrix.SetIdentity();
    RLOG(LogLevel::Info, "----->> \n{}", GlobalConstants.ViewProjMatrix.ToString());

    RCommandContext* Context = GGraphicInterface->BeginCommandContext("MainRender"); 
    
    // 临时做法，后面整个Present过程应该和场景渲染过程解耦
    RGraphicViewport* Viewport = GGraphicInterface->GetViewport();  
    
    ID3D12GraphicsCommandList* CommandList = Context->GetCommandList(); 

    CommandList->SetGraphicsRootSignature(m_rootSignature.Get());
    CommandList->RSSetViewports(1, &m_viewport);
    CommandList->RSSetScissorRects(1, &m_scissorRect);


    auto PresentBufferTransition = CD3DX12_RESOURCE_BARRIER::Transition(Viewport->GetCurrentRT(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    CommandList->ResourceBarrier(1, &PresentBufferTransition);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = Viewport->GetCurrentRTViewHandle();  
    CommandList->OMSetRenderTargets(1,&rtvHandle,0,nullptr);
    
    const float clearColor[] = {0.0f, 0.9f,0.0f, 1.0f};
    CommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr); 
    CommandList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    CommandList->SetPipelineState(m_pipelineState.Get());
    Context->SetDynamicConstantBufferView(0, sizeof(ObjectConstants), &GlobalConstants);

    CommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    CommandList->DrawInstanced(6, 1, 0, 0);

    CommandList->SetGraphicsRootSignature(m_NewPSO->GetRootSignature());
    CommandList->SetPipelineState(m_NewPSO->GetPipelineState());
    Context->SetDynamicConstantBufferView(0, sizeof(ObjectConstants), &GlobalConstants);
    //Context->SetDynamicConstantBufferView(1, sizeof(ObjectConstants), &GlobalConstants);
    Context->SetDynamicConstantBufferView(2, sizeof(ObjectConstants), &GlobalConstants);
    for (size_t i = 0; i < InPrims.size(); ++i)
    {
        std::shared_ptr<RPrimitiveObject> obj = InPrims[i];
        if (RModelData* data = obj->GetModelData())
        {
            const std::vector<RMeshData>&  Meshes = data->GetMeshesData();
            for (const RMeshData& Mesh : Meshes)
            {
                D3D12_GPU_VIRTUAL_ADDRESS buffer_ptr = data->GetGeometryDataBuffer().GetGPUVirtualAddress();
                D3D12_VERTEX_BUFFER_VIEW buffer_view = { buffer_ptr + Mesh.vbOffset, Mesh.vbSize , Mesh.vbStride };
                CommandList->IASetVertexBuffers(0, 1, &buffer_view); 
                D3D12_INDEX_BUFFER_VIEW indexBufferView = { buffer_ptr + Mesh.ibOffset, Mesh.ibSize, DXGI_FORMAT_R32_UINT }; 
                CommandList->IASetIndexBuffer(&indexBufferView);
                CommandList->DrawIndexedInstanced(Mesh.indexCount, 1, 0, 0, 0);
            }
        }
    }

    auto RT_Present = CD3DX12_RESOURCE_BARRIER::Transition(Viewport->GetCurrentRT(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT); 
    CommandList->ResourceBarrier(1, &RT_Present);
    return Context->Finish(); 
}

void D3DApp::OnRender(const RViewInfo& View)  
{
    RCommandListManager* QueueMgr = GGraphicInterface->GetCommandListManager();
    QueueMgr->WaitForFence(FrameAsyncFence);
    FrameAsyncFence = QueueMgr->GetGraphicsQueue().IncrementFence();

    PopulateCommandListNew(View); 
    
    GGraphicInterface->Present();
}
// https://www.braynzarsoft.net/viewtutorial/q16390-04-direct3d-12-drawing

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

std::wstring D3DApp::GetShaderPath() const 
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::wcout << "current path : " << currentPath.generic_wstring() << std::endl;
    return currentPath.generic_wstring() + L"/../../../RealEngine/Render/";
    // return  L"/sdf";
}

