#include "CommandContext.h"
#include "RHIBuffer.h"
#include "GraphicInterface.h"
#include "CommandListManager.h"

RCommandContext* RCommandContextManger::AllocateContext(D3D12_COMMAND_LIST_TYPE Type) 
{
    std::lock_guard<std::mutex> LockGuard(m_ContextAllocationMutex); 
    std::queue<RCommandContext*>& AvailableQueue = m_AvailableContexts[Type]; 
    RCommandContext* Ret = nullptr; 
    if (AvailableQueue.empty()) 
    {
        Ret = new RCommandContext(Type); 
        m_ContextPool[Type].emplace_back(Ret); 
        Ret->Initialize(); 
    }
    else
    {
        Ret = AvailableQueue.front();
        AvailableQueue.pop();
        Ret->Reset();
    }
    return Ret;
}

void RCommandContextManger::FreeContext(RCommandContext* InContext)
{
    assert(InContext != nullptr);
    std::lock_guard<std::mutex> LockGuard(m_ContextAllocationMutex);
    m_AvailableContexts[InContext->GetContextType()].push(InContext);
}

void RCommandContextManger::DestroyAllContexts()
{
    for (size_t i = 0; i < 4; i++)
    {
        m_ContextPool[i].clear();
    }
}

void RCommandContext::Initialize()
{
    RCommandListManager* CMDManager = GGraphicInterface->GetCommandListManager();
    CMDManager->CreateNewCommandList(m_Type, &m_CommandList, &m_CurrentAllocator);
}

uint64_t RCommandContext::Finish(bool WaitForCompletion) 
{
    // TODO:
    RCommandListManager* CMDManager = GGraphicInterface->GetCommandListManager();
    RCommandQueue& Queue = CMDManager->GetQueue(m_Type);
    
    uint64_t CompleteFence = Queue.ExecuteCommandList(m_CommandList);

    Queue.DiscardAllocator(CompleteFence, m_CurrentAllocator);
    m_CurrentAllocator = nullptr;

    //if (WaitForCompletion)
        //CMDManager.WaitForFence();
    GGraphicInterface->GetCommandContextManger()->FreeContext(this);
    return 0;
}

RRHIBuffer *RCommandContext::CreateBuffer(const void *Data, uint32_t Size, uint32_t Stride, std::string_view DebugName)
{
    D3D12_HEAP_PROPERTIES UploadHeapProps = GetUploadBufferHeapProps();
    D3D12_RESOURCE_DESC ResourceDesc = GetUploadBufferResourceDesc(Size);
    
    Microsoft::WRL::ComPtr<ID3D12Resource> UploadBuffer;
    ASSERT(mDevice->CreateCommittedResource(&UploadHeapProps,D3D12_HEAP_FLAG_NONE,&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(&UploadBuffer)));
    void* MapedData;
    CD3DX12_RANGE MapedRange(0,Size);
    UploadBuffer->Map(0,&MapedRange, &MapedData);
    memcpy(MapedData, Data, Size);
    UploadBuffer->Unmap(0,&MapedRange);

    D3D12_RESOURCE_STATES InitialState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
    RRHIBuffer* NewBuffer= new RRHIBuffer(Size/Stride,Stride,Size,InitialState,DebugName);

    CD3DX12_HEAP_PROPERTIES DefaultHeapDesc(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC NewBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(Size,D3D12_RESOURCE_FLAG_NONE,0);

    ASSERT(mDevice->CreateCommittedResource(&DefaultHeapDesc,D3D12_HEAP_FLAG_NONE,
        &NewBufferDesc,InitialState, nullptr, IID_PPV_ARGS(&(NewBuffer->mResource))));

    TransitionResource(NewBuffer, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,true);
    mCommandList->CopyBufferRegion(NewBuffer->GetResource(), 0, UploadBuffer.Get(), 0, Size);
    TransitionResource(NewBuffer, D3D12_RESOURCE_STATE_GENERIC_READ,true);

    return NewBuffer;
}

D3D12_HEAP_PROPERTIES RCommandContext::GetUploadBufferHeapProps() const
{
    static D3D12_HEAP_PROPERTIES UploadHeap = 
    {
        D3D12_HEAP_TYPE_UPLOAD,
        D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
        D3D12_MEMORY_POOL_UNKNOWN,
        1,
        1
    };
    return UploadHeap;
}

D3D12_RESOURCE_DESC RCommandContext::GetUploadBufferResourceDesc(uint32_t BufferSize) const
{
    static D3D12_RESOURCE_DESC Desc={
        D3D12_RESOURCE_DIMENSION_BUFFER,
        0,
        BufferSize,
        1,
        1,
        1,
        DXGI_FORMAT_UNKNOWN,
        DXGI_SAMPLE_DESC{1,0},
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
        D3D12_RESOURCE_FLAG_NONE
    };
    return Desc;
}

// https://learn.microsoft.com/en-us/windows/win32/direct3d12/using-resource-barriers-to-synchronize-resource-states-in-direct3d-12
void RCommandContext::TransitionResource(RRHIBuffer* Buffer, D3D12_RESOURCE_STATES NewState, bool FlushImmediate)
{
    D3D12_RESOURCE_STATES OldState = Buffer->mUsage;
    if(OldState != NewState)
    {
        CD3DX12_RESOURCE_BARRIER BarrierDesc = 
            CD3DX12_RESOURCE_BARRIER::Transition(Buffer->GetResource(),OldState, NewState,
            D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_BARRIER_FLAG_NONE);
        Buffer->mUsage = NewState;

        mCommandList->ResourceBarrier(1, &BarrierDesc);
    }
}

