#pragma once

#include <vector>
#include <queue>
#include <mutex>

#include "RHIResource.h"

struct RDynamicAlloc
{

};
// 表示一大块GPU内存
class RLinearAllocationPage : public RRHIResource
{
public:
	RLinearAllocationPage(ID3D12Resource* pResource, D3D12_RESOURCE_STATES Usage)
		: RRHIResource()
	{
		// 仅仅使内部的ptr_成为pResource, 不会增加pResource的引用计数，
		// 适用于将一个从DX12接口新创建的对象变成ComPtr管理
		// 原来的指针不为空则会调用它的释放
		m_Resource.Attach(pResource);
		m_UsageState = Usage;
		m_GpuVirtualAddress = m_Resource->GetGPUVirtualAddress();
		m_Resource->Map(0, nullptr, &m_CpuVirtualAddress);
	}
	~RLinearAllocationPage()
	{
		Unmap();
	}
	void Map()
	{
		if (m_CpuVirtualAddress == nullptr)
		{
			m_Resource->Map(0, nullptr, &m_CpuVirtualAddress);
		}
	}
	void Unmap()
	{
		if (m_CpuVirtualAddress != nullptr)
		{
			m_Resource->Unmap(0, nullptr);
			m_CpuVirtualAddress = nullptr;
		}
	}
private:
	void* m_CpuVirtualAddress = nullptr;
};
enum ELinearAllocatorType
{
	kInvalidAllocator = -1, 
	
	kGpuExclusive = 0,
	kCpuWritable = 1,

	kNumAllocatorTypes
};
class RLinearAllocatorPageManager
{
public:
	RLinearAllocatorPageManager(ELinearAllocatorType InType);
	std::shared_ptr<RLinearAllocationPage> RequestPage();
	std::shared_ptr<RLinearAllocationPage> CreateNewPage(size_t PageSize = 0);
	void DiscardPages(uint64_t FenceID, const std::vector<std::shared_ptr<RLinearAllocationPage>>& Pages);
	void FreeLargePages(uint64_t FenceID, const std::vector<std::shared_ptr<RLinearAllocationPage>>& Pages); 

private:
	static constexpr uint32_t DefaultGpuLllocatorPageSize = 0x10000; 
	static constexpr uint32_t DefaultCpuAllocatorPageSize = 0x200000; 

	ELinearAllocatorType m_AllocationType;
	std::vector<std::shared_ptr<RLinearAllocationPage>> m_PagePool;
	std::queue<std::pair<uint64_t, std::shared_ptr<RLinearAllocationPage>>> m_RetiredPages;
	std::queue<std::pair<uint64_t, std::shared_ptr<RLinearAllocationPage>>> m_DeletionQueue;
	std::queue<std::shared_ptr<RLinearAllocationPage>> m_AvailablePages;
	std::mutex m_Mutex;
};
