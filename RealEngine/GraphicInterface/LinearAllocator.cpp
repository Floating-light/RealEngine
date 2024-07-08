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

std::shared_ptr<RLinearAllocationPage> RLinearAllocatorPageManager::CreateNewPage(size_t PageSize)
{
	std::shared_ptr<RLinearAllocationPage> RetVal = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Device> LocalDevice = GGraphicInterface->GetDevice();

	D3D12_HEAP_PROPERTIES HeapProps={};
	HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	HeapProps.CreationNodeMask = 1;
	HeapProps.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC ResourceDesc={};
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Alignment = 0;
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_RESOURCE_STATES DefaultUsage;
	
	if (m_AllocationType == ELinearAllocatorType::kGpuExclusive)
	{
		HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
		ResourceDesc.Width = PageSize == 0 ? DefaultGpuLllocatorPageSize : PageSize;
		ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		DefaultUsage = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	}
	else
	{
		HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
		ResourceDesc.Width = PageSize == 0 ? DefaultCpuAllocatorPageSize : PageSize;
		ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		DefaultUsage = D3D12_RESOURCE_STATE_GENERIC_READ;
	}
	ID3D12Resource* NewBuffer={};
	ASSERT(LocalDevice->CreateCommittedResource(&HeapProps,D3D12_HEAP_FLAG_NONE,&ResourceDesc, DefaultUsage, nullptr, IID_PPV_ARGS(&NewBuffer)));
	NewBuffer->SetName(L"LinearAllocator Page");

	RetVal = std::shared_ptr<RLinearAllocationPage>(new RLinearAllocationPage(NewBuffer, DefaultUsage));
	return RetVal;
}

void RLinearAllocatorPageManager::DiscardPages(uint64_t FenceID, const std::vector<std::shared_ptr<RLinearAllocationPage>>& Pages)
{

}
