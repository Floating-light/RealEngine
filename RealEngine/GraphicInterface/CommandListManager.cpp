#include "CommandListManager.h"

RCommandQueue::RCommandQueue(D3D12_COMMAND_LIST_TYPE InType): 
	m_CommandQueue(nullptr),
	m_Type(InType),
	m_AllocatorPool(InType),
	m_Fence(nullptr),
	// 避免冲突，初始化高位为根据不同类型CommandList递增的值，| 1就是+1，
	m_NextFenceValue((uint64_t)InType << 56 | 1),
	m_LastCompletedFenceValue((uint64_t)InType << 56)
{
}

RCommandQueue::~RCommandQueue()
{
	ShutDown();
}

void RCommandQueue::Create(ID3D12Device* InDevice) 
{
	assert(InDevice);
	assert(!IsReady());
	assert(m_AllocatorPool.Size() == 0);

	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = m_Type;
	QueueDesc.NodeMask = 0x1;// 多GPU情况下，在第几个GPU上执行，
	assert(SUCCEEDED(InDevice->CreateCommandQueue(&QueueDesc,IID_PPV_ARGS(&m_CommandQueue))));
	m_CommandQueue->SetName(L"CommandListManager::m_CommandQueue");

	assert(SUCCEEDED(InDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))));
	m_Fence->SetName(L"CommandListManager::m_Fence");
	m_Fence->Signal(m_LastCompletedFenceValue);

	m_FenceEventHandle = CreateEvent(nullptr, false, false, nullptr);
	assert(m_FenceEventHandle != NULL);

	m_AllocatorPool.Create(InDevice);

	assert(IsReady());
}

void RCommandQueue::ShutDown()
{
	if (m_CommandQueue == nullptr)
		return;
	// 与初始化的顺序相反
	m_AllocatorPool.Shutdown();

	CloseHandle(m_FenceEventHandle);

	m_Fence->Release();
	m_Fence = nullptr;

	m_CommandQueue->Release();
	m_CommandQueue = nullptr;
}

uint64_t RCommandQueue::ExecuteCommandList(ID3D12CommandList* InList)
{
	std::lock_guard<std::mutex> lock_gurad(m_FenceMutex);
	assert(SUCCEEDED(((ID3D12GraphicsCommandList*)InList)->Close())); 

	m_CommandQueue->ExecuteCommandLists(1, &InList);

	m_CommandQueue->Signal(m_Fence, m_NextFenceValue);

	return m_NextFenceValue++;
}

ID3D12CommandAllocator* RCommandQueue::RequestAllocator(void) 
{
	uint64_t CurrentCompeletedFence= m_Fence->GetCompletedValue();
	return m_AllocatorPool.RequestAllocator(CurrentCompeletedFence);
}

void RCommandQueue::DiscardAllocator(uint64_t FenceValueForReset, ID3D12CommandAllocator* InAllocator) 
{
	m_AllocatorPool.DiscardAllocator(FenceValueForReset, InAllocator);
}

RCommandListManager::RCommandListManager():
	m_Device(nullptr),
	m_GraphicsQueue(D3D12_COMMAND_LIST_TYPE_DIRECT),
	m_ComputeQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE),
	m_CopyQueue(D3D12_COMMAND_LIST_TYPE_COPY)
{
}

RCommandListManager::~RCommandListManager()
{
	ShutDown();
}

void RCommandListManager::Create(ID3D12Device* InDevice) 
{
	m_Device = InDevice;

	m_GraphicsQueue.Create(InDevice);
	m_ComputeQueue.Create(InDevice);
	m_CopyQueue.Create(InDevice); 
}

void RCommandListManager::ShutDown() 
{
	m_GraphicsQueue.ShutDown();
	m_ComputeQueue.ShutDown();
	m_CopyQueue.ShutDown();
}

void RCommandListManager::CreateNewCommandList(D3D12_COMMAND_LIST_TYPE InType, ID3D12GraphicsCommandList** List, ID3D12CommandAllocator** Allocator) 
{
	assert(InType != D3D12_COMMAND_LIST_TYPE_BUNDLE);

	RCommandQueue& Que = GetQueue(InType);
	*Allocator = Que.RequestAllocator(); 
	assert(SUCCEEDED(m_Device->CreateCommandList(1,InType,*Allocator, nullptr, IID_PPV_ARGS(List)))); 
	(*List)->SetName(L"MyCommandList");
}
