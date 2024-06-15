#pragma once 
#include "D3D12ThirdPart.h"
#include "CommandAllocatorPool.h"

class RCommandQueue
{
	friend class RCommandListManager;
	friend class RCommandContext;

	RCommandQueue(D3D12_COMMAND_LIST_TYPE InType);
	~RCommandQueue();

	void Create(ID3D12Device* InDevice); 
	void ShutDown(); 
public:
	bool IsReady() { return m_CommandQueue != nullptr; };

	bool IsFenceComplete(uint64_t FenceValue);
	void WaitForFence(uint64_t FenceValue);

	ID3D12CommandQueue* GetCommandQueue()const { return m_CommandQueue; }; 
private:
	uint64_t ExecuteCommandList(ID3D12CommandList* InList);
	ID3D12CommandAllocator* RequestAllocator(void);
	void DiscardAllocator(uint64_t FenceValueForReset, ID3D12CommandAllocator* InAllocator);

	ID3D12CommandQueue* m_CommandQueue;

	const D3D12_COMMAND_LIST_TYPE m_Type;
	
	RCommandAllocatorPool m_AllocatorPool;

	std::mutex m_FenceMutex;
	std::mutex m_EventMutex;

	ID3D12Fence* m_Fence;
	uint64_t m_NextFenceValue;
	uint64_t m_LastCompletedFenceValue;
	HANDLE m_FenceEventHandle;
};
class RCommandListManager
{
public:
	RCommandListManager();
	~RCommandListManager();

	void Create(ID3D12Device* InDevice);
	void ShutDown(); 

	RCommandQueue& GetGraphicsQueue() { return m_GraphicsQueue; };
	RCommandQueue& GetComputeQueue() { return m_ComputeQueue; };
	RCommandQueue& GetCopyQueue() { return m_CopyQueue; };
	RCommandQueue& GetQueue(D3D12_COMMAND_LIST_TYPE InType);
	// 创建SwapChain需要它
	ID3D12CommandQueue* GetCommandQueue()const { return m_GraphicsQueue.GetCommandQueue(); };

	void CreateNewCommandList(D3D12_COMMAND_LIST_TYPE InType, ID3D12GraphicsCommandList** List, ID3D12CommandAllocator** Allocator);
	void WaitForFence(uint64_t FenceValue);
private:
	ID3D12Device* m_Device; 
	RCommandQueue m_GraphicsQueue;
	RCommandQueue m_ComputeQueue;
	RCommandQueue m_CopyQueue;
};

inline RCommandQueue& RCommandListManager::GetQueue(D3D12_COMMAND_LIST_TYPE InType)
{
	switch (InType)
	{
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		return m_ComputeQueue;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		return m_CopyQueue;
	default:
		return m_GraphicsQueue;
	}
}