#include "LinearAllocator.h"
#include "GraphicInterface.h"
#include "CommandListManager.h"

RLinearAllocatorPageManager::RLinearAllocatorPageManager(ELinearAllocatorType InType)
	: m_AllocationType(InType)
{
}

std::shared_ptr<RLinearAllocationPage> RLinearAllocatorPageManager::RequestPage()
{
	std::lock_guard<std::mutex> lockGuard(m_Mutex);
	
	RCommandListManager* Mgr = GGraphicInterface->GetCommandListManager();
	// 先检查退休的Page有没有已经使用完毕可以复用的
	while (!m_RetiredPages.empty() && Mgr->IsFenceComplete(m_RetiredPages.front().first)) 
	{
		m_AvailablePages.push(m_RetiredPages.front().second);
		m_RetiredPages.pop();
	}

	std::shared_ptr<RLinearAllocationPage> RetPage = nullptr;
	if (!m_AvailablePages.empty())
	{
		RetPage = m_AvailablePages.front();
		m_AvailablePages.pop();
	}
	else
	{
		RetPage = CreateNewPage(); 
		m_PagePool.emplace_back(RetPage);
	}

	return RetPage;
}
