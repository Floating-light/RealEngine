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

	void SetRenderTargetFormats(uint32_t NumRTVs, const DXGI_FORMAT* RTVFormats, DXGI_FORMAT DSVFormat, uint32_t MsaaCount = 1, uint32_t MsaaQuality = 0);
	void SetSampleMask(uint32_t InMask);
	void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE InTopologyType);
	void SetRasterizer(const D3D12_RASTERIZER_DESC& InRasterizer);
	void SetDepthStencil(const D3D12_DEPTH_STENCIL_DESC& InDepthStencil);
	void SetBlendState(const D3D12_BLEND_DESC& InBlendDesc);
	void SetShader(const std::string& VSShaderPath, const std::string& PSShaderPath);
	void SetInputLayout(UINT NumElements, const D3D12_INPUT_ELEMENT_DESC* pInputElementDescs);
	void Finalize();
private:
	Microsoft::WRL::ComPtr<ID3DBlob> VSShader;
	Microsoft::WRL::ComPtr<ID3DBlob> PSShader;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PSODesc;
	std::shared_ptr<const D3D12_INPUT_ELEMENT_DESC> m_InputLayouts;
};