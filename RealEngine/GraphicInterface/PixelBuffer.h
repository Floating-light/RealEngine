#pragma once

#include "RHIResource.h"

// Texture类资源，有着宽，高，Dimension，格式 等属性
class RPixelBuffer : public RRHIResource
{
public:
	RPixelBuffer():
		m_Width(0), 
		m_Height(0), 
		m_ArraySize(0), 
		m_Format(DXGI_FORMAT_UNKNOWN) 
	{};
	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	uint32_t GetDepth() const { return m_ArraySize; }
	DXGI_FORMAT GetFormat() const { return m_Format; }

	void ExportToFile(const std::string& FilePath);
protected:
	D3D12_RESOURCE_DESC DescribeTexture2D(uint32_t InWidth, uint32_t InHeight, uint32_t DepthOrArraySize, uint32_t NumMips, DXGI_FORMAT Format, uint32_t Flags);
	void AssociateWithResource(ID3D12Device* Device, const std::string& InName, ID3D12Resource* InResource, D3D12_RESOURCE_STATES CurrentState);
	void CreateTextureResource(ID3D12Device* Device, const std::string& InName, const D3D12_RESOURCE_DESC& InResourceDesc,
		D3D12_CLEAR_VALUE ClearValue, D3D12_GPU_VIRTUAL_ADDRESS VitualAddress = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN);

	static DXGI_FORMAT GetBaseFormat(DXGI_FORMAT Format);
	static DXGI_FORMAT GetStencilFormat(DXGI_FORMAT InFormat);
	static DXGI_FORMAT GetDSVFormat(DXGI_FORMAT InFormat);
	static DXGI_FORMAT GetDepthFormat(DXGI_FORMAT Format);

private:
	uint32_t m_Width;
	uint32_t m_Height;
	uint32_t m_ArraySize;
	DXGI_FORMAT m_Format;
};
