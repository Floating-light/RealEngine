#include "RootSignature.h"
#include "GraphicInterface.h"
void RRootSignature::Finalize(const std::string& name, D3D12_ROOT_SIGNATURE_FLAGS Flags)
{
	assert(!m_Finalized);
	D3D12_ROOT_SIGNATURE_DESC RootDesc;
	RootDesc.NumParameters = 0;
	RootDesc.pParameters = nullptr;
	RootDesc.NumStaticSamplers = 0;
	RootDesc.pStaticSamplers = nullptr;
	RootDesc.Flags = Flags;

	using Microsoft::WRL::ComPtr;
	ComPtr<ID3DBlob> pOutBlob;
	ComPtr<ID3DBlob> pErrorBlob;
	ASSERT(D3D12SerializeRootSignature(&RootDesc, D3D_ROOT_SIGNATURE_VERSION_1, pOutBlob.GetAddressOf(), pErrorBlob.GetAddressOf()));
	ID3D12Device* Device = GGraphicInterface->GetDevice().Get(); 
	ASSERT(Device->CreateRootSignature(1, pOutBlob->GetBufferPointer(), pOutBlob->GetBufferSize(), IID_PPV_ARGS(&m_Signature))); 
	m_Signature->SetName(RUtility::StringToWstring(name).c_str());

	m_Finalized = true;
}
