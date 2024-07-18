#include "Render.h"

#include <filesystem>

#include "GraphicInterface.h"
#include "Logging.h"

#include "RootSignature.h"
#include "PipelineState.h"
#include "GraphicViewport.h"
#include "CommandContext.h"
#include "PrimitiveInfo.h"
#include "ModelData.h"

#include "D3DApp.h"
// for test purpose
#include "GenericPlatform/GenericWindow.h"

static std::string NewGetShaderPath()
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    return currentPath.string() + "/../../../RealEngine/Render/";
}

RRenderer& RRenderer::Get()
{
    static RRenderer Renderer;
    return Renderer;
}
static D3DApp* TestApp = nullptr;
void RRenderer::Init(std::shared_ptr<RGenericWindow> Window)
{
    TestApp = new D3DApp();
    TestApp->InitializeViewport(Window->GetWindowHandle(),1280, 720,L"Test");
    TestApp->Setup();

    GGraphicInterface->InitilizeViewport(HWND(Window->GetWindowHandle()), 1280, 720); 
    RGraphicViewport* Viewprot = GGraphicInterface->GetViewport();

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
    DepthState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    DepthState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    DepthState.BackFace = DepthState.FrontFace;

    m_NewPSO->SetDepthStencil(DepthState);
    m_NewPSO->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    m_NewPSO->SetSampleMask((std::numeric_limits<uint32_t>::max)());
    DXGI_FORMAT RtFormat = Viewprot->GetSwapChainFormat();  
    m_NewPSO->SetRenderTargetFormats(1, &RtFormat, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN); 
    m_NewPSO->Finalize(); 
}

void RRenderer::DoRender(RViewInfo& ViewInfo)
{

    TestApp->OnUpdate(0.1);
    TestApp->OnRender(ViewInfo);

    const std::vector<std::shared_ptr<RPrimitiveObject>>& InPrims = ViewInfo.GetPrimitives();

    ObjectConstants GlobalConstants;
    GlobalConstants.ViewProjMatrix = ViewInfo.GetViewProjectionMatrix();
    //GlobalConstants.ViewProjMatrix.SetIdentity();
    RLOG(LogLevel::Info, "----->> \n{}", GlobalConstants.ViewProjMatrix.ToString());

    RCommandContext* Context = GGraphicInterface->BeginCommandContext("MainRender");

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
    CommandList->OMSetRenderTargets(1, &rtvHandle, 0, nullptr);

    const float clearColor[] = { 0.0f, 0.9f,0.0f, 1.0f };
    CommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    CommandList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Context->SetDynamicConstantBufferView(0, sizeof(ObjectConstants), &GlobalConstants);


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
            const std::vector<RMeshData>& Meshes = data->GetMeshesData();
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
    Context->Finish();

}

void RRenderer::Destroy()
{
    if (TestApp)
    {
        delete TestApp;
        TestApp = nullptr;
    }
}
