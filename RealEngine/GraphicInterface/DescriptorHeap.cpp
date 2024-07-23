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

//  Descriptor Heap for descriptor table

void RDescriptorHeap::Create(const std::string& InName, D3D12_DESCRIPTOR_HEAP_TYPE InType, int32_t InMaxCount)
{
	ID3D12Device* Device = GGraphicInterface->GetDeviceRaw();

	m_HeapDesc.Type = InType;
	m_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	m_HeapDesc.NodeMask = 1;
	m_HeapDesc.NumDescriptors = InMaxCount;

	ASSERTDX(Device->CreateDescriptorHeap(&m_HeapDesc, IID_PPV_ARGS(m_DescriptorHeap.ReleaseAndGetAddressOf())));
	m_DescriptorHeap->SetName(RUtility::StringToWstring(InName).c_str());

	m_DescriptorHandleIncrementSize = Device->GetDescriptorHandleIncrementSize(InType);
	m_NumFreeDescriptors = InMaxCount;

	m_FirstHandle.m_CpuHandle = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(); 
	m_FirstHandle.m_GpuHandle = m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	m_NextFreeHandle = m_FirstHandle;
}

RDescriptorHandle RDescriptorHeap::Allocate(uint32_t InCount) 
{
	RCHECK(HasAvailableSpace(InCount)); 
	m_NumFreeDescriptors -= InCount;
	RDescriptorHandle RetVal = m_NextFreeHandle;

	m_NextFreeHandle.m_CpuHandle.ptr +=  m_DescriptorHandleIncrementSize * InCount; 
	m_NextFreeHandle.m_GpuHandle.ptr +=  m_DescriptorHandleIncrementSize * InCount;  

	return RetVal;
}
