#include "PipelineState.h"
#include "RootSignature.h"
#include "GraphicInterface.h"

RGraphicPSO::RGraphicPSO(const std::string& InName) 
	: RPSO(InName)
{
	memset(&m_PSODesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	m_PSODesc.NodeMask = 1;
	m_PSODesc.SampleMask = 0xFFFFFFFFu;
	m_PSODesc.SampleDesc.Count = 1;
	m_PSODesc.InputLayout.NumElements = 0;
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
	assert(m_PSODesc.pRootSignature);

	ID3D12Device* Device = GGraphicInterface->GetDevice().Get();
	assert(m_PSODesc.DepthStencilState.DepthEnable != (m_PSODesc.DSVFormat == DXGI_FORMAT_UNKNOWN)); 
	ASSERT(Device->CreateGraphicsPipelineState(&m_PSODesc, IID_PPV_ARGS(&m_PSO)));
	m_PSO->SetName(RUtility::StringToWstring(m_Name).c_str()); 
}
