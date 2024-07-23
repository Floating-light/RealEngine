#pragma once 
#include "D3D12ThirdPart.h"

// 无限容量的DescriptorHeap，用于在资源初始化的时候创建它对应的View。
// 需要绑定到渲染管线的Descritpor应该被Copy到对应的Heap
class RDescriptorAllocator
{
public:
	RDescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE Type)
		: m_Type(Type)
		, m_CurrentDescriptorHeap(nullptr) 
		, m_CurrentHandle({D3D12_GPU_VIRTUAL_ADDRESS_NULL})
		{};
	D3D12_CPU_DESCRIPTOR_HANDLE Allocate(uint32_t Count);
private:
	void RequestNewHeap();
	static constexpr uint32_t m_NumDescriptorsPerHeap = 256;
	// Device上的创建方法都不是线程安全的，包括CreateDescriptorHeap。
	static std::mutex m_AllocationMutex; 
	std::vector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> m_DescriptorHeapPool;

	D3D12_DESCRIPTOR_HEAP_TYPE m_Type;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_CurrentDescriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_CurrentHandle;
	uint32_t m_DescriptorSize = 0;;
	uint32_t m_RemainingFreeHandle = 0;
};

class RDescriptorHandle
{
	friend class RDescriptorHeap;
public:
	RDescriptorHandle()
	{
		m_CpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_GpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}
	RDescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE InCpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE InGpuHandle)
		: m_CpuHandle(InCpuHandle)
		, m_GpuHandle(InGpuHandle)
	{}

	RDescriptorHandle operator+ (int32_t InOffsetScaledByDescriptorSize) const
	{
		RDescriptorHandle Ret = *this;
		if (Ret.m_CpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		{
			Ret.m_CpuHandle.ptr += InOffsetScaledByDescriptorSize;
		}
		if (Ret.m_GpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		{
			Ret.m_GpuHandle.ptr += InOffsetScaledByDescriptorSize;
		}
	}

	const D3D12_CPU_DESCRIPTOR_HANDLE* operator&() const { return &m_CpuHandle; }
	operator D3D12_CPU_DESCRIPTOR_HANDLE() const { return m_CpuHandle; }
	operator D3D12_GPU_DESCRIPTOR_HANDLE() const { return m_GpuHandle; }
private:
	D3D12_CPU_DESCRIPTOR_HANDLE m_CpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GpuHandle;
};

class RDescriptorHeap
{
public:
	RDescriptorHeap() {};
	~RDescriptorHeap() { Destroy(); }
	void Destroy() { m_DescriptorHeap = nullptr; }; 

	void Create(const std::string& InName, D3D12_DESCRIPTOR_HEAP_TYPE Type, int32_t InMaxCount);
	RDescriptorHandle Allocate(uint32_t InCount = 1);
	bool HasAvailableSpace(uint32_t InCount) { return InCount <= m_NumFreeDescriptors; }
	ID3D12DescriptorHeap* GetDescriptorHeap() const { return m_DescriptorHeap.Get(); }
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DescriptorHeap;
	D3D12_DESCRIPTOR_HEAP_DESC m_HeapDesc;
	uint32_t m_DescriptorHandleIncrementSize;
	uint32_t m_NumFreeDescriptors;

	RDescriptorHandle m_FirstHandle;
	RDescriptorHandle m_NextFreeHandle;
};