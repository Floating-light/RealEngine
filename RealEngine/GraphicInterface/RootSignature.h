#pragma once

#include "D3D12ThirdPart.h"

class RRootSignature
{
public:
	ID3D12RootSignature* GetRootSignature()const { return m_Signature.Get(); }
	void AddAsConstantBuffer(uint8_t Register, D3D12_SHADER_VISIBILITY Visibility, uint8_t Space = 0);

	void Finalize(const std::string& name, D3D12_ROOT_SIGNATURE_FLAGS Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);

private:
	bool m_Finalized = false;
	std::vector<D3D12_ROOT_PARAMETER> m_Params;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_Signature;
};