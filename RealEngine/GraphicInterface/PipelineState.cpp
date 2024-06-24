#include "PipelineState.h"
#include "RootSignature.h"

RGraphicPSO::RGraphicPSO(const std::string& InName) 
	: RPSO(InName)
{
	memset(&m_PSODesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	m_PSODesc.NodeMask = 1;
	m_PSODesc.SampleMask = 0xFFFFFFFFu;
	m_PSODesc.SampleDesc.Count = 1;
	m_PSODesc.InputLayout.NumElements = 0;
}

void RGraphicPSO::Finalize()
{
	m_PSODesc.pRootSignature = m_RootSignature->GetRootSignature();
	assert(m_PSODesc.pRootSignature);

	m_PSODesc.InputLayout.pInputElementDescs = nullptr;

}
