#include "Render.h"

#include <filesystem>

#include "GraphicInterface.h"
#include "Logging.h"

#include "RootSignature.h"
#include "PipelineState.h"
#include "CommandListManager.h"
#include "GraphicViewport.h"
#include "CommandContext.h"
#include "PrimitiveInfo.h"
#include "ModelData.h"
#include "SamplerDesc.h"

// for test purpose
#include "GenericPlatform/GenericWindow.h"

static constexpr DXGI_FORMAT DSV_FORMAT = DXGI_FORMAT_D32_FLOAT;

__declspec(align(256)) struct ObjectConstants
{
    Matrix4 ViewProjMatrix;
};

static std::string NewGetShaderPath()
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    return currentPath.string() + "/../../../RealEngine/Render/";
}

struct LocalRGBA
{
    uint8_t R, G, B, A;
};
std::vector<LocalRGBA> CreateProceduralTex(int32_t Width, int32_t Height)
{
    std::vector<LocalRGBA> RetVal(Width * Height);
    uint64_t checkSize = 80;
    for (size_t y = 0; y < Height; ++y)
    {
        for (size_t x = 0; x < Width; ++x)
        {
            bool isWhite = ((x / checkSize) % 2 == ((y / checkSize) % 2));
            uint8_t color = isWhite ? 255 : 0;
            //RetVal[y * Width + Height] = {color, color, color, 0};
            RetVal[y * Width + Height] = {255, 0, 0, 255};
        }
    }
    return RetVal;
}

RRenderer& RRenderer::Get()
{
    static RRenderer Renderer;
    return Renderer;
}
void RRenderer::Init(std::shared_ptr<RGenericWindow> Window)
{
    ID3D12Device* Device = GGraphicInterface->GetDeviceRaw();
    GGraphicInterface->InitilizeViewport(HWND(Window->GetWindowHandle()), 1280, 720); 
    RGraphicViewport* Viewprot = GGraphicInterface->GetViewport();
    m_TextureHeap.Create("GlobalTextureHeap", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 4096);

    m_SceneDepthBuffer.Create("SceneDepthBuffer", 1280, 720, DSV_FORMAT);
    std::vector<LocalRGBA> ColorData = CreateProceduralTex(1280, 720);
    //m_DefaultTexture.Create2D("DefaultTexture", 1280 * sizeof(LocalRGBA), 1280, 720, DXGI_FORMAT_R8G8B8A8_UNORM, ColorData.data()); 
    
    m_DefaultTexture = RTexture::CreateTexture("../../resources/HuangQuan/衣内.png"); 
    
    {   
        D3D12_CPU_DESCRIPTOR_HANDLE defaultTextureCpuHandle = m_DefaultTexture.GetSRV(); 
        
        m_DefaultTextureHandle = m_TextureHeap.Allocate(1); 
        const uint32_t NumHandle = 1; 
        //Device->CopyDescriptors(1, &m_DefaultTextureHandle, &NumHandle, 1, &defaultTextureCpuHandle, &NumHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        Device->CopyDescriptorsSimple(1, m_DefaultTextureHandle, defaultTextureCpuHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV); 
    }


    std::shared_ptr< RRootSignature> NewSignature = std::shared_ptr<RRootSignature>(new RRootSignature());
    NewSignature->Reset(4); 
    RSamplerDesc samplerDesc{};
    
    NewSignature->InitStaticSampler(0, samplerDesc, D3D12_SHADER_VISIBILITY_PIXEL);
    NewSignature->SetParamAsConstantBuffer(0, 0, D3D12_SHADER_VISIBILITY_VERTEX); 
    NewSignature->SetParamAsConstantBuffer(1, 0, D3D12_SHADER_VISIBILITY_PIXEL);
    NewSignature->SetParamAsConstantBuffer(2, 1, D3D12_SHADER_VISIBILITY_ALL);
    NewSignature->SetParamAsDescriptorRange(3,D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1,D3D12_SHADER_VISIBILITY_PIXEL); // 先给材质占10个位置

    NewSignature->Finalize("MyNewRootSignature", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    m_DefaultPSO = std::shared_ptr<RGraphicPSO>(new RGraphicPSO("MyNewPSO"));
    D3D12_INPUT_ELEMENT_DESC Desces[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
    };
    m_DefaultPSO->SetInputLayout(3, Desces);
    m_DefaultPSO->SetShader(NewGetShaderPath() + "BaseVS.hlsl", NewGetShaderPath() + "BasePS.hlsl");
    m_DefaultPSO->SetRootSignature(NewSignature);
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
    alphaBlend.RenderTarget[0].LogicOpEnable = false;
    alphaBlend.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_CLEAR;

    m_DefaultPSO->SetBlendState(alphaBlend);

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

    m_DefaultPSO->SetRasterizer(rasterizerDesc);

    D3D12_DEPTH_STENCIL_DESC DepthState = {};
    DepthState.DepthEnable = true;
    DepthState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    //DepthState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
    DepthState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; 
    DepthState.StencilEnable = false;
    DepthState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    DepthState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
    DepthState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    DepthState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    DepthState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    DepthState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    DepthState.BackFace = DepthState.FrontFace;
    
    m_DefaultPSO->SetDepthStencil(DepthState);
    m_DefaultPSO->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    m_DefaultPSO->SetSampleMask((std::numeric_limits<uint32_t>::max)());
    DXGI_FORMAT RtFormat = Viewprot->GetSwapChainFormat();  
    m_DefaultPSO->SetRenderTargetFormats(1, &RtFormat, m_SceneDepthBuffer.GetFormat()); 
    m_DefaultPSO->Finalize(); 

}

void RRenderer::DoRender(RViewInfo& ViewInfo)
{
    RCommandListManager* QueueMgr = GGraphicInterface->GetCommandListManager();
    QueueMgr->WaitForFence(m_FrameAsyncFence);
    m_FrameAsyncFence = QueueMgr->GetGraphicsQueue().IncrementFence();

    const std::vector<std::shared_ptr<RPrimitiveObject>>& InPrims = ViewInfo.GetPrimitives();

    ObjectConstants GlobalConstants;
    GlobalConstants.ViewProjMatrix = ViewInfo.GetViewProjectionMatrix();
    //GlobalConstants.ViewProjMatrix.SetIdentity();
    //RLOG(LogLevel::Info, "----->> \n{}", GlobalConstants.ViewProjMatrix.ToString());

    RCommandContext* Context = GGraphicInterface->BeginCommandContext("MainRender");
    Context->TransitionResource(m_SceneDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
    Context->ClearDepth(m_SceneDepthBuffer);

    // 临时做法，后面整个Present过程应该和场景渲染过程解耦
    RGraphicViewport* Viewport = GGraphicInterface->GetViewport();
    D3D12_VIEWPORT CurViewport = Viewport->GetViewportSize();
    D3D12_RECT CurScissor = Viewport->GetScissor();

    ID3D12GraphicsCommandList* CommandList = Context->GetCommandList();

    //CommandList->SetGraphicsRootSignature(m_rootSignature.Get());
    CommandList->RSSetViewports(1, &CurViewport);
    CommandList->RSSetScissorRects(1, &CurScissor);


    auto PresentBufferTransition = CD3DX12_RESOURCE_BARRIER::Transition(Viewport->GetCurrentRT(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    CommandList->ResourceBarrier(1, &PresentBufferTransition);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = Viewport->GetCurrentRTViewHandle();
    //CommandList->OMSetRenderTargets(1, &rtvHandle, 0, nullptr);
    Context->SetRenderTarget(rtvHandle, m_SceneDepthBuffer.GetDSV());

    const float clearColor[] = { 0.0f, 0.9f,0.0f, 1.0f };
    CommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    CommandList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    CommandList->SetGraphicsRootSignature(m_DefaultPSO->GetRootSignature());
    CommandList->SetPipelineState(m_DefaultPSO->GetPipelineState());

    // RootIndex 指的是构建RootSignature时，第几个D3D12_ROOT_PARAMETER
    //Context->SetDynamicConstantBufferView(1, sizeof(ObjectConstants), &GlobalConstants);
    Context->SetDynamicConstantBufferView(2, sizeof(ObjectConstants), &GlobalConstants);

    ID3D12DescriptorHeap* descHeaps[] = { m_TextureHeap.GetDescriptorHeap()};
    CommandList->SetDescriptorHeaps(1, descHeaps);
    for (size_t i = 0; i < InPrims.size(); ++i)
    {
        std::shared_ptr<RPrimitiveObject> obj = InPrims[i];
        Context->SetConstantBuffer(0, obj->GetModelConstant());
        if (RModelData* data = obj->GetModelData())
        {
            const std::vector<RMeshData>& Meshes = data->GetMeshesData();
            for (const RMeshData& Mesh : Meshes)
            {
                //if (Mesh.MaterialIndex == 2 || Meshes.size() == 1)
                {
                    CommandList->SetGraphicsRootDescriptorTable(3, data->GetMaterialSRV(Mesh.MaterialIndex));

                    D3D12_GPU_VIRTUAL_ADDRESS buffer_ptr = data->GetGeometryDataBuffer().GetGPUVirtualAddress();
                    D3D12_VERTEX_BUFFER_VIEW buffer_view = { buffer_ptr + Mesh.vbOffset, Mesh.vbSize , Mesh.vbStride };
                    CommandList->IASetVertexBuffers(0, 1, &buffer_view);
                    D3D12_INDEX_BUFFER_VIEW indexBufferView = { buffer_ptr + Mesh.ibOffset, Mesh.ibSize, DXGI_FORMAT_R32_UINT };
                    CommandList->IASetIndexBuffer(&indexBufferView); 
                    CommandList->DrawIndexedInstanced(Mesh.indexCount, 1, 0, 0, 0);
                }
            }
        }
    }

    auto RT_Present = CD3DX12_RESOURCE_BARRIER::Transition(Viewport->GetCurrentRT(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    CommandList->ResourceBarrier(1, &RT_Present);
    Context->Finish();

    GGraphicInterface->Present(); 
}

void RRenderer::Destroy()
{
}

RDescriptorHandle RRenderer::AllocateSRV(uint32_t Count)
{
    return m_TextureHeap.Allocate(Count);
}
