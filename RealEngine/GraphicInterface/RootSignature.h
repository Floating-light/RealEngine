#pragma once

#include "D3D12ThirdPart.h"

class RRootParameter
{
	friend class RRootSignature;
public:
	RRootParameter()
	{
		m_RootParam.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
	}
	~RRootParameter()
	{
		Clear();
	}
	void Clear()
	{
		if (m_RootParam.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
		{
			delete[] m_RootParam.DescriptorTable.pDescriptorRanges;
		}
		m_RootParam.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
	}

	void InitAsConstantBuffer(uint32_t Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL, uint32_t Space = 0);
	void InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE Type, uint32_t Register, uint32_t Count, D3D12_SHADER_VISIBILITY Visiblity= D3D12_SHADER_VISIBILITY_ALL, uint32_t Space = 0);

	void InitAsDescriptorTable(uint32_t RangeCount, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetTableRange(uint32_t RangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE Type, uint32_t Register, uint32_t Count, uint32_t Space = 0);

	const D3D12_ROOT_PARAMETER& operator()() const { return m_RootParam; }
private:
	D3D12_ROOT_PARAMETER m_RootParam{}; 
};

class RRootSignature
{
public:
	ID3D12RootSignature* GetRootSignature()const { return m_Signature.Get(); }

	void Reset(uint32_t Num) { m_Params.resize(Num); }

	void SetParamAsConstantBuffer(uint32_t ParamIndex, uint32_t Register, D3D12_SHADER_VISIBILITY Visibility, uint32_t Space = 0);
	void SetParamAsDescriptorRange(uint32_t ParamIndex, D3D12_DESCRIPTOR_RANGE_TYPE Type, uint32_t Register, uint32_t Count,
		D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL, uint32_t Space = 0);

	void Finalize(const std::string& name, D3D12_ROOT_SIGNATURE_FLAGS Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);

private:
	bool m_Finalized = false;
	std::vector<RRootParameter> m_Params; 
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_Signature;
};