#pragma once 
#include <string>
#include "D3D12ThirdPart.h"
class RRootSignature;

class RPSO
{
public:
	RPSO(const std::string& str)
		: name(str)
		, m_RootSignature(nullptr)
		, m_PSO(nullptr)
	{

	}
	void SetRootSignature(std::shared_ptr<RRootSignature> InSignature)
	{
		m_RootSignature = InSignature;
	}
protected:
	std::string name;

	std::shared_ptr<RRootSignature> m_RootSignature;  
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PSO;
};

class RGraphicPSO : public RPSO
{
public:
	RGraphicPSO(const std::string& InName);
	void Finalize();
private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PSODesc;
	std::shared_ptr<const D3D12_INPUT_ELEMENT_DESC> m_InputLayout;
};