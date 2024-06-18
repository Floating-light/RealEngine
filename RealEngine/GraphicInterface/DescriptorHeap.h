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