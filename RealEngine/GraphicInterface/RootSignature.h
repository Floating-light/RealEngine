#pragma once

#include "D3D12ThirdPart.h"

class RRootSignature
{
public:
	ID3D12RootSignature* GetRootSignature()const { return m_Signature.Get(); }
	void Finalize(const std::string& name, D3D12_ROOT_SIGNATURE_FLAGS Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);
private:
	bool m_Finalized = false;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_Signature;
};