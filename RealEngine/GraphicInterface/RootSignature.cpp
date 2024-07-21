#include "RootSignature.h"
#include "GraphicInterface.h"

void RRootParameter::InitAsConstantBuffer(uint32_t Register, D3D12_SHADER_VISIBILITY Visibility, uint32_t Space)
{
	m_RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	m_RootParam.ShaderVisibility = Visibility;
	m_RootParam.Descriptor.ShaderRegister = Register;
	m_RootParam.Descriptor.RegisterSpace = Space;
}

void RRootParameter::InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE Type, uint32_t Register, uint32_t Count, D3D12_SHADER_VISIBILITY Visiblity, uint32_t Space)
{
	InitAsDescriptorTable(1, Visiblity);
	SetTableRange(0, Type, Register, Count, Space);
}

void RRootParameter::InitAsDescriptorTable(uint32_t RangeCount, D3D12_SHADER_VISIBILITY Visibility)
{
	m_RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	m_RootParam.ShaderVisibility = Visibility;
	m_RootParam.DescriptorTable.NumDescriptorRanges = RangeCount;
	m_RootParam.DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[RangeCount];
}

void RRootParameter::SetTableRange(uint32_t RangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE Type, uint32_t Register, uint32_t Count, uint32_t Space)
{
	RCHECK(RangeIndex < m_RootParam.DescriptorTable.NumDescriptorRanges);
	D3D12_DESCRIPTOR_RANGE* CurRange = const_cast<D3D12_DESCRIPTOR_RANGE*>(&m_RootParam.DescriptorTable.pDescriptorRanges[RangeIndex]); 
	CurRange->RangeType = Type;
	CurRange->BaseShaderRegister = Register;
	CurRange->NumDescriptors = Count;
	CurRange->RegisterSpace= Space;
}

void RRootSignature::SetParamAsConstantBuffer(uint32_t ParamIndex, uint32_t Register, D3D12_SHADER_VISIBILITY Visibility, uint32_t Space)
{
	RCHECK(ParamIndex < m_Params.size());
	m_Params[ParamIndex].InitAsConstantBuffer(Register, Visibility, Space);
}

void RRootSignature::SetParamAsDescriptorRange(uint32_t ParamIndex, D3D12_DESCRIPTOR_RANGE_TYPE Type, uint32_t Register, 
	uint32_t Count, D3D12_SHADER_VISIBILITY Visibility, uint32_t Space)
{
	RCHECK(ParamIndex < m_Params.size());
	m_Params[ParamIndex].InitAsDescriptorRange(Type, Register, Count, Visibility, Space); 
}

void RRootSignature::Finalize(const std::string& name, D3D12_ROOT_SIGNATURE_FLAGS Flags)
{
	assert(!m_Finalized);
	D3D12_ROOT_SIGNATURE_DESC RootDesc = {};
	RootDesc.NumParameters = m_Params.size(); 
	RootDesc.pParameters = (const D3D12_ROOT_PARAMETER*)m_Params.data(); 
	RootDesc.NumStaticSamplers = 0;
	RootDesc.pStaticSamplers = nullptr;
	RootDesc.Flags = Flags;

	using Microsoft::WRL::ComPtr;
	ComPtr<ID3DBlob> pOutBlob;
	ComPtr<ID3DBlob> pErrorBlob;
	ASSERTDX(D3D12SerializeRootSignature(&RootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pOutBlob, &pErrorBlob));
	ID3D12Device* Device = GGraphicInterface->GetDevice().Get(); 
	ASSERTDX(Device->CreateRootSignature(1, pOutBlob->GetBufferPointer(), pOutBlob->GetBufferSize(), IID_PPV_ARGS(&m_Signature))); 
	m_Signature->SetName(RUtility::StringToWstring(name).c_str());

	m_Finalized = true;
}

