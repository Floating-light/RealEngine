#include "CommandAllocatorPool.h"

#include <sstream>

RCommandAllocatorPool::RCommandAllocatorPool(D3D12_COMMAND_LIST_TYPE Type) :
	m_CommandListType(Type),
	m_Device(nullptr)
{
}

RCommandAllocatorPool::~RCommandAllocatorPool()
{
	Shutdown();
}

void RCommandAllocatorPool::Create(ID3D12Device* InDevice)
{
	m_Device = InDevice;
}

void RCommandAllocatorPool::Shutdown()
{
	for (int i = 0; i < m_AllocatorPool.size(); ++i)
	{
		m_AllocatorPool[i]->Release(); 
	}
	m_AllocatorPool.clear();
}

ID3D12CommandAllocator* RCommandAllocatorPool::RequestAllocator(uint64_t CompletedFenceValue)
{
	std::lock_guard<std::mutex> LockGurad(m_AllocatorMutex); 
	ID3D12CommandAllocator* Ret = nullptr;

	if (!m_ReadyAllocators.empty())
	{
		std::pair<uint64_t, ID3D12CommandAllocator*> AllocatorPair = m_ReadyAllocators.front(); 
		if (AllocatorPair.first <= CompletedFenceValue)
		{
			Ret = AllocatorPair.second;
			Ret->Reset();
			m_ReadyAllocators.pop();
		}
	}
	if (!Ret)
	{
		if (FAILED(m_Device->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&Ret))))
		{
			RLOG(LogLevel::Fatal, "Create command allocator failed ");
		}
		
		std::wstringstream ss; 
		ss << L"REngine CommandAllocator " << m_AllocatorPool.size(); 
		Ret->SetName(ss.str().c_str()); 

		m_AllocatorPool.push_back(Ret);
	}

	return Ret; 
}

void RCommandAllocatorPool::DiscardAllocator(uint64_t FenceValue, ID3D12CommandAllocator* Allocator)
{
	std::lock_guard<std::mutex> LockGurad(m_AllocatorMutex); 
	m_ReadyAllocators.push({ FenceValue, Allocator });
}
