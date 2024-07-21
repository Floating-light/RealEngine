#include "PipelineState.h"
#include "RootSignature.h"
#include "GraphicInterface.h"

ID3D12RootSignature* RPSO::GetRootSignature() const
{
	return m_RootSignature->GetRootSignature();
}

RGraphicPSO::RGraphicPSO(const std::string& InName) 
	: RPSO(InName)
{
	memset(&m_PSODesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	m_PSODesc.NodeMask = 1;
	m_PSODesc.SampleMask = 0xFFFFFFFFu;
	m_PSODesc.SampleDesc.Count = 1;
	m_PSODesc.InputLayout.NumElements = 0;
}
void RGraphicPSO::SetRenderTargetFormats(uint32_t NumRTVs, const DXGI_FORMAT* RTVFormats, DXGI_FORMAT DSVFormat, uint32_t MsaaCount, uint32_t MsaaQuality)
{
	for (size_t i = 0; i < NumRTVs; ++i)
	{
		m_PSODesc.RTVFormats[i] = RTVFormats[i];
	}
	for (size_t i = NumRTVs; i < m_PSODesc.NumRenderTargets; i++)
	{
		m_PSODesc.RTVFormats[i] = DXGI_FORMAT_UNKNOWN; 
	}
	m_PSODesc.NumRenderTargets = NumRTVs;
	m_PSODesc.DSVFormat = DSVFormat;
	m_PSODesc.SampleDesc.Count = MsaaCount;
	m_PSODesc.SampleDesc.Quality = MsaaQuality;
}
void RGraphicPSO::SetSampleMask(uint32_t InMask)
{
	m_PSODesc.SampleMask = InMask; 
}
void RGraphicPSO::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE InTopologyType)
{
	m_PSODesc.PrimitiveTopologyType = InTopologyType;
}
void RGraphicPSO::SetRasterizer(const D3D12_RASTERIZER_DESC& InRasterizer)
{
	m_PSODesc.RasterizerState = InRasterizer;
}
void RGraphicPSO::SetDepthStencil(const D3D12_DEPTH_STENCIL_DESC& InDepthStencil)
{
	m_PSODesc.DepthStencilState = InDepthStencil;
}
void RGraphicPSO::SetBlendState(const D3D12_BLEND_DESC& InBlendDesc)
{
	m_PSODesc.BlendState = InBlendDesc;
}

void RGraphicPSO::SetShader(const std::string& VSShaderPath, const std::string& PSShaderPath)
{
	VSShader.Reset();
	PSShader.Reset();
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ASSERTDX(D3DCompileFromFile(RUtility::StringToWstring(VSShaderPath).c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &VSShader, nullptr));
	ASSERTDX(D3DCompileFromFile(RUtility::StringToWstring(PSShaderPath).c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &PSShader, nullptr));

	m_PSODesc.VS = D3D12_SHADER_BYTECODE{ VSShader->GetBufferPointer(), VSShader->GetBufferSize() };
	m_PSODesc.PS = D3D12_SHADER_BYTECODE{ PSShader->GetBufferPointer(), PSShader->GetBufferSize() };

}

void RGraphicPSO::SetInputLayout(UINT NumElements, const D3D12_INPUT_ELEMENT_DESC* pInputElementDescs)
{
	m_PSODesc.InputLayout.NumElements = NumElements; 
	if (NumElements > 0)
	{
		D3D12_INPUT_ELEMENT_DESC* NewElements = (D3D12_INPUT_ELEMENT_DESC*)malloc(sizeof(D3D12_INPUT_ELEMENT_DESC) * NumElements);
		memcpy(NewElements, pInputElementDescs, sizeof(D3D12_INPUT_ELEMENT_DESC) * NumElements);
		m_InputLayouts.reset(NewElements);
	}
	else
	{
		m_InputLayouts = nullptr;
	}
}

void RGraphicPSO::Finalize()
{
	m_PSODesc.pRootSignature = m_RootSignature->GetRootSignature();
	RCHECK(m_PSODesc.pRootSignature);

	m_PSODesc.InputLayout.pInputElementDescs = m_InputLayouts.get(); 

	ID3D12Device* Device = GGraphicInterface->GetDevice().Get();
	RCHECK(m_PSODesc.DepthStencilState.DepthEnable != (m_PSODesc.DSVFormat == DXGI_FORMAT_UNKNOWN)); 
	ASSERTDX(Device->CreateGraphicsPipelineState(&m_PSODesc, IID_PPV_ARGS(&m_PSO)));
	m_PSO->SetName(RUtility::StringToWstring(m_Name).c_str()); 
}
