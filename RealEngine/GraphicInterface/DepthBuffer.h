#pragma once

#include "PixelBuffer.h"

// 用于DepthBuffer的Texture
class RDepthBuffer : public RPixelBuffer
{
public:
	RDepthBuffer(float ClearDepth = 1.f,uint8_t ClearStencil = 0) 
		: m_ClearDepth(ClearDepth)
		, m_ClearStencil(ClearStencil)
	{
		m_DSV[0].ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_DSV[1].ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_DSV[2].ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_DSV[3].ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_DepthSRV.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_StencilSRV.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}

	// 创建DepthBuffer
	void Create(const std::string& InName, uint32_t InWidth, uint32_t InHeight, DXGI_FORMAT InFormat);
	void Create(const std::string& InName, uint32_t InWidth, uint32_t InHeight, uint32_t InNumSamples, DXGI_FORMAT InFormat);

	float GetClearDepth()const { return m_ClearDepth; }
	uint8_t GetClearStencil() const { return m_ClearStencil; }

	// 要想Draw的时候能更新DepthBuffer，不能用ReadOnly的View
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSV()const { return m_DSV[0]; };

	D3D12_CPU_DESCRIPTOR_HANDLE GetDSV_DepthReadOnly() { return m_DSV[1]; };
private:
	// 创建DepthBuffer的View
	void CreateDerivedViews(ID3D12Device* Device, DXGI_FORMAT InFormat) ;

	float m_ClearDepth;
	uint8_t m_ClearStencil;
	D3D12_CPU_DESCRIPTOR_HANDLE m_DSV[4];
	D3D12_CPU_DESCRIPTOR_HANDLE m_DepthSRV;
	D3D12_CPU_DESCRIPTOR_HANDLE m_StencilSRV;
};