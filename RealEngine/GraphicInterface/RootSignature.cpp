#include "RootSignature.h"
#include "GraphicInterface.h"

void RRootSignature::AddAsConstantBuffer(uint8_t Register, D3D12_SHADER_VISIBILITY Visibility, uint8_t Space )
{
	D3D12_ROOT_PARAMETER param = {};
	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; 
	param.ShaderVisibility = Visibility;
	param.Descriptor.ShaderRegister = Register;
	param.Descriptor.RegisterSpace = Space;
	m_Params.push_back(param);
}

void RRootSignature::Finalize(const std::string& name, D3D12_ROOT_SIGNATURE_FLAGS Flags)
{
	assert(!m_Finalized);
	D3D12_ROOT_SIGNATURE_DESC RootDesc = {};
	RootDesc.NumParameters = m_Params.size(); 
	RootDesc.pParameters = m_Params.data();
	RootDesc.NumStaticSamplers = 0;
	RootDesc.pStaticSamplers = nullptr;
	RootDesc.Flags = Flags;

	using Microsoft::WRL::ComPtr;
	ComPtr<ID3DBlob> pOutBlob;
	ComPtr<ID3DBlob> pErrorBlob;
	ASSERT(D3D12SerializeRootSignature(&RootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pOutBlob, &pErrorBlob));
	ID3D12Device* Device = GGraphicInterface->GetDevice().Get(); 
	ASSERT(Device->CreateRootSignature(1, pOutBlob->GetBufferPointer(), pOutBlob->GetBufferSize(), IID_PPV_ARGS(&m_Signature))); 
	m_Signature->SetName(RUtility::StringToWstring(name).c_str());

	m_Finalized = true;
}
