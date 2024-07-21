#include "DepthBuffer.h"
#include "GraphicInterface.h"

void RDepthBuffer::Create(const std::string& InName, uint32_t InWidth, uint32_t InHeight, DXGI_FORMAT InFormat)
{
	Create(InName, InWidth, InHeight, 1, InFormat);
}

void RDepthBuffer::Create(const std::string& InName, uint32_t InWidth, uint32_t InHeight, uint32_t InNumSamples, DXGI_FORMAT InFormat)
{
	D3D12_RESOURCE_DESC desc = DescribeTexture2D(InWidth, InHeight, 1, 1, InFormat, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	desc.SampleDesc.Count = InNumSamples;

	D3D12_CLEAR_VALUE ClearValue = {};
	ClearValue.Format = InFormat;
	ClearValue.DepthStencil.Depth = 1.f;

	CreateTextureResource(GGraphicInterface->GetDeviceRaw(), InName, desc, ClearValue);
	CreateDerivedViews(GGraphicInterface->GetDeviceRaw(), InFormat);
}

void RDepthBuffer::CreateDerivedViews(ID3D12Device* Device, DXGI_FORMAT InFormat)
{
	ID3D12Resource* Resource = m_Resource.Get();

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = GetDSVFormat(InFormat);
	if (Resource->GetDesc().SampleDesc.Count == 1) // 多重采样 MSAA，
	{
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
	}
	else
	{
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS; // 多重采样的 DSV
	}

	if (m_DSV[0].ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
	{
		// Allocate form global heap 
		m_DSV[0] = GGraphicInterface->AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_DSV); 
		m_DSV[1] = GGraphicInterface->AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}
	
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	Device->CreateDepthStencilView(Resource, &dsvDesc, m_DSV[0]);

	dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH;
	Device->CreateDepthStencilView(Resource, &dsvDesc, m_DSV[1]);

	DXGI_FORMAT stencilReadFormat = GetStencilFormat(InFormat);
	if (stencilReadFormat != DXGI_FORMAT_UNKNOWN)
	{
		if (m_DSV[2].ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		{
			m_DSV[2] = GGraphicInterface->AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			m_DSV[3] = GGraphicInterface->AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		}

		dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_STENCIL;
		Device->CreateDepthStencilView(Resource, &dsvDesc, m_DSV[2]);
		dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH | D3D12_DSV_FLAG_READ_ONLY_STENCIL; 
		Device->CreateDepthStencilView(Resource, &dsvDesc, m_DSV[3]);
	}
	else
	{
		m_DSV[2] = m_DSV[0];
		m_DSV[3] = m_DSV[1];
	}

	// Create Depht SRV
	if (m_DepthSRV.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
	{
		m_DepthSRV = GGraphicInterface->AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = GetDepthFormat(InFormat);
	if (dsvDesc.ViewDimension == D3D12_DSV_DIMENSION_TEXTURE2D)
	{
		SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;
	}
	else
	{
		SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
	}
	// 定义Texture各个通道的映射方式，
	// D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING表示 R->R,G->G,B->B,A->A
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	Device->CreateShaderResourceView(Resource, &SRVDesc, m_DepthSRV);

	// 有两种情况，D24S8和D32S8
	// 这里不乱哪一种情况Stencil都是在PlaneSlice = 1
	// Create Stencil SRV
	if (stencilReadFormat != DXGI_FORMAT_UNKNOWN)
	{
		if (m_StencilSRV.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		{
			m_StencilSRV = GGraphicInterface->AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		}

		SRVDesc.Format = stencilReadFormat;
		// DepthStencil 是一种多平面格式，这里指定要读取的平面。DXGI_FORMAT_NV12也是。
		// 别的格式没这功能，HLSL中可以直接Load后，通过rgba访问。
		SRVDesc.Texture2D.PlaneSlice = 1;

		Device->CreateShaderResourceView(Resource, &SRVDesc, m_StencilSRV); 
	}
}
