#include "DescriptorHeap.h"
#include "GraphicInterface.h"

std::mutex RDescriptorAllocator::m_AllocationMutex;

D3D12_CPU_DESCRIPTOR_HANDLE RDescriptorAllocator::Allocate(uint32_t Count)
{
	if (m_CurrentDescriptorHeap == nullptr || m_RemainingFreeHandle < Count)
	{
		RequestNewHeap();
	}
	D3D12_CPU_DESCRIPTOR_HANDLE RetHandle = m_CurrentHandle;
	m_CurrentHandle.ptr = RetHandle.ptr + Count * m_DescriptorSize;
	m_RemainingFreeHandle -= Count;
	return RetHandle; 
}

void RDescriptorAllocator::RequestNewHeap() 
{

	Microsoft::WRL::ComPtr<ID3D12Device> Device = GGraphicInterface->GetDevice(); 
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = m_Type;
	desc.NumDescriptors = m_NumDescriptorsPerHeap;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 1;

	m_CurrentDescriptorHeap = nullptr;
	{
		std::lock_guard<std::mutex> lock(m_AllocationMutex); 
		Device->CreateDescriptorHeap(&desc,IID_PPV_ARGS(&m_CurrentDescriptorHeap)); 
	}
	m_DescriptorHeapPool.push_back(m_CurrentDescriptorHeap); 

	m_RemainingFreeHandle = m_NumDescriptorsPerHeap;
	m_CurrentHandle = m_CurrentDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	if (m_DescriptorSize == 0)
	{
		m_DescriptorSize = Device->GetDescriptorHandleIncrementSize(m_Type);
	}
}
