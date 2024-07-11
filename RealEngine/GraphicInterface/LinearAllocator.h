#pragma once

#include <vector>
#include <queue>
#include <mutex>

#include "RHIResource.h"


struct RDynamicAlloc
{
public:
	RDynamicAlloc(RRHIResource& InResource, size_t InOffset, size_t InSize)
		: Resource(InResource)
		, Offset(InOffset)
		, Size(InSize) 
		, DataPtr(nullptr)
		, GpuAddress(D3D12_GPU_VIRTUAL_ADDRESS_NULL)
	{};

	RRHIResource& Resource;
	size_t Offset;
	size_t Size;
	void* DataPtr ;
	D3D12_GPU_VIRTUAL_ADDRESS GpuAddress ;
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
	void* GetMapedCpuVirtualAddress() { return m_CpuVirtualAddress; }
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
	void Destroy();

	static constexpr uint32_t DefaultGpuLllocatorPageSize = 0x10000; // 64 K
	static constexpr uint32_t DefaultCpuAllocatorPageSize = 0x200000; // 2MB
private:

	ELinearAllocatorType m_AllocationType;
	std::queue<std::pair<uint64_t, std::shared_ptr<RLinearAllocationPage>>> m_RetiredPages;
	std::queue<std::pair<uint64_t, std::shared_ptr<RLinearAllocationPage>>> m_DeletionQueue;
	std::queue<std::shared_ptr<RLinearAllocationPage>> m_AvailablePages;
	std::mutex m_Mutex;
};

class RLinearAllocator
{
public:
	static constexpr uint32_t DEFAULT_ALIGN = 256; 
	RLinearAllocator(ELinearAllocatorType Type) 
		: m_AllocationType(Type) 
		, m_PageSize(0)
		, m_CurOffset(~(size_t)0)
		, m_CurPages(nullptr)
	{
		assert(Type > ELinearAllocatorType::kInvalidAllocator && Type < ELinearAllocatorType::kNumAllocatorTypes);
		m_PageSize = (m_AllocationType == ELinearAllocatorType::kGpuExclusive) 
			? RLinearAllocatorPageManager::DefaultGpuLllocatorPageSize 
			: RLinearAllocatorPageManager::DefaultCpuAllocatorPageSize;
	};

	RDynamicAlloc Allocate(size_t SizeInBytes, size_t Alignment = DEFAULT_ALIGN);
	void CleanupUsedPages(uint64_t FenceID);
private:
	RDynamicAlloc AllocateLargePage(size_t SizeInBytes);

	ELinearAllocatorType m_AllocationType;
	size_t m_PageSize;
	size_t m_CurOffset;
	std::shared_ptr<RLinearAllocationPage> m_CurPages;
	std::vector<std::shared_ptr<RLinearAllocationPage>> m_RetiredPages;
	std::vector<std::shared_ptr<RLinearAllocationPage>> m_LargePageList;
};
