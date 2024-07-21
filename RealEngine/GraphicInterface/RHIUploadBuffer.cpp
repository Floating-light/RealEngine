#include "RHIUploadBuffer.h"
#include "GraphicInterface.h"

void RRHIUploadBuffer::Create(const std::string& name, size_t BufferSize)
{
	Destroy();
	m_BufferSize = BufferSize;

	Microsoft::WRL::ComPtr<ID3D12Device> Device = GGraphicInterface->GetDevice(); 

	D3D12_HEAP_PROPERTIES Heap = {};
	Heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	Heap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	Heap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	Heap.CreationNodeMask = 1;
	Heap.VisibleNodeMask = 1;
	
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = m_BufferSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1; 
	desc.SampleDesc.Quality = 0; 
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ASSERTDX(Device->CreateCommittedResource(&Heap,D3D12_HEAP_FLAG_NONE,&desc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr, IID_PPV_ARGS(&m_Resource))); 

	m_GpuVirtualAddress = m_Resource->GetGPUVirtualAddress();
	m_Resource->SetName(RUtility::StringToWideString(name).c_str());
}

void* RRHIUploadBuffer::Map()
{
	void* Memory;
	D3D12_RANGE Range = {};
	Range.Begin = 0;
	Range.End = m_BufferSize;
	m_Resource->Map(0, &Range, &Memory);
	return Memory;
}

void RRHIUploadBuffer::Unmap(size_t begin, size_t end) 
{
	D3D12_RANGE Range = {begin, (std::min)(end, m_BufferSize)};
	m_Resource->Unmap(0, &Range);
}
