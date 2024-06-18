#include "RHIBuffer.h"
#include "GraphicInterface.h"
void RRHIBuffer::Create(const std::string& name, uint32_t NumElements, uint32_t ElementSize, const RRHIUploadBuffer& SrcData, uint32_t srcOffset)
{
	Destroy();

	m_ElementCount = NumElements;
	m_ElementSize = ElementSize;
	m_BufferSize = NumElements * ElementSize;

	D3D12_RESOURCE_DESC desc = CreateBufferDescribe();
	
	m_UsageState = D3D12_RESOURCE_STATE_COMMON;

	D3D12_HEAP_PROPERTIES HeapProps = {};
	HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	HeapProps.CreationNodeMask = 1;
	HeapProps.VisibleNodeMask = 1;

	Microsoft::WRL::ComPtr<ID3D12Device> Device = GGraphicInterface->GetDevice();
	ASSERT(Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &desc, m_UsageState, nullptr, IID_PPV_ARGS(&m_Resource))); 

	m_GpuVirtualAddress = m_Resource->GetGPUVirtualAddress();

	GGraphicInterface->InitializeBuffer(*this, SrcData, srcOffset);

	m_Resource->SetName(RUtility::StringToWstring(name).c_str());

	CreateViewsDerived(); 
}

D3D12_RESOURCE_DESC RRHIBuffer::CreateBufferDescribe()
{
	assert(m_BufferSize != 0);
	D3D12_RESOURCE_DESC desc = {};
	desc.Alignment = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags = m_ResourceFlag;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Height = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width = (uint64_t)m_BufferSize;
	return desc;
}

void RRHIBufferByteAddress::CreateViewsDerived()
{
	Microsoft::WRL::ComPtr<ID3D12Device> LocalDevice= GGraphicInterface->GetDevice();
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	SRVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SRVDesc.Buffer.NumElements = (UINT)m_BufferSize / 4;
	SRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW; 

	if (m_SRV.ptr == D3D12_GPU_VIRTUAL_ADDRESS_NULL)
	{
		m_SRV = GGraphicInterface->AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV); 
	}
	LocalDevice->CreateShaderResourceView(m_Resource.Get(), &SRVDesc, m_SRV);

	D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
	UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	UAVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	UAVDesc.Buffer.NumElements = (UINT)m_BufferSize / 4;
	UAVDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;

	if (m_UAV.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
	{
		m_UAV = GGraphicInterface->AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
	// 第二个参数 原子操作计数
	LocalDevice->CreateUnorderedAccessView(m_Resource.Get(), nullptr, &UAVDesc, m_UAV);
}
