#pragma once

#include "RHIResource.h"
#include <filesystem>

class RTexture : public RRHIResource
{
public:
	RTexture()
	{
		m_CpuDescriptorHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}
	RTexture(D3D12_CPU_DESCRIPTOR_HANDLE InHandle)
		: m_CpuDescriptorHandle(InHandle) {}
	virtual void Destroy() override 
	{
		RRHIResource::Destroy();
		m_CpuDescriptorHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}

	static RTexture CreateTexture(const std::filesystem::path InPath); 
	void Create2D(const std::string& InName, uint32_t RowPitchBytes, uint32_t InWidth, uint32_t InHeight, DXGI_FORMAT InFormat, const void* InData);
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV() const { return m_CpuDescriptorHandle; }
	uint32_t GetWidth()const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	uint32_t GetDepth() const { return m_Depth; }
private:

	uint32_t m_Width{0};
	uint32_t m_Height{0};
	uint32_t m_Depth{0};

	D3D12_CPU_DESCRIPTOR_HANDLE m_CpuDescriptorHandle;
};