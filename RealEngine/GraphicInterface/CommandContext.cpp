#include "CommandContext.h"
#include "RHIBuffer.h"

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
