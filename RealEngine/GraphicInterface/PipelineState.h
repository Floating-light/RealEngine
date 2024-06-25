#pragma once 
#include <string>
#include "D3D12ThirdPart.h"
class RRootSignature;

class RPSO
{
public:
	RPSO(const std::string& str)
		: m_Name(str)
		, m_RootSignature(nullptr)
		, m_PSO(nullptr)
	{

	}
	void SetRootSignature(std::shared_ptr<RRootSignature> InSignature)
	{
		m_RootSignature = InSignature;
	}
protected:
	std::string m_Name;

	std::shared_ptr<RRootSignature> m_RootSignature;  
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PSO;
};

class RGraphicPSO : public RPSO
{
public:
	RGraphicPSO(const std::string& InName);

	void SetShader(const std::string& VSShaderPath, const std::string& PSShaderPath);
	void SetInputLayout(UINT NumElements, const D3D12_INPUT_ELEMENT_DESC* pInputElementDescs);
	void Finalize();
private:
	Microsoft::WRL::ComPtr<ID3DBlob> VSShader;
	Microsoft::WRL::ComPtr<ID3DBlob> PSShader;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PSODesc;
	std::shared_ptr<const D3D12_INPUT_ELEMENT_DESC> m_InputLayouts;
};