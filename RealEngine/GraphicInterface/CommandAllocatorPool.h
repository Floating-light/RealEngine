#pragma once 
#include <vector>
#include <queue>
#include "D3D12ThirdPart.h"

class RCommandAllocatorPool
{
public:
	RCommandAllocatorPool(D3D12_COMMAND_LIST_TYPE Type);
	~RCommandAllocatorPool();

	void Create(ID3D12Device* InDevice);
	void Shutdown();

	// 请求Allocator时，CompletedFenceValue为当前Queue的同步Fence值，表明当前Queue执行到的Fence点
	// 只会复用同步Fence点大于等于CompletedFenceValue的Allocator
	ID3D12CommandAllocator* RequestAllocator(uint64_t CompletedFenceValue);
	// 当Allocator关联的CommandList执行后，插入同步的Fence点，这个Fence值就是Allocator的同步点，这里记下这个点和对应Allocator
	// 后续，直接传入当前Queue fence的值，即可判断对应的Allocator是否可以释放，复用。
	void DiscardAllocator(uint64_t FenceValue, ID3D12CommandAllocator* Allocator);
	inline size_t Size() { return m_AllocatorPool.size(); } 
private:
	ID3D12Device* m_Device;
	D3D12_COMMAND_LIST_TYPE m_CommandListType;
	std::vector<ID3D12CommandAllocator*> m_AllocatorPool;
	// fence -> allocator, allocator 使用的fence
	std::queue<std::pair<uint64_t, ID3D12CommandAllocator*>> m_ReadyAllocators;
	std::mutex m_AllocatorMutex;
};