#include "CommandContext.h"
#include "RHIBuffer.h"

RRHIBuffer *RCommandContext::CreateBuffer(const void *Data, uint32_t Size, uint32_t Stride, std::string_view DebugName)
{
    D3D12_HEAP_PROPERTIES UploadHeapProps = GetUploadBufferHeapProps();
    D3D12_RESOURCE_DESC ResourceDesc = GetUploadBufferResourceDesc(Size);
    
    Microsoft::WRL::ComPtr<ID3D12Resource> UploadBuffer;
    ASSERT(mDevice->CreateCommittedResource(&UploadHeapProps,D3D12_HEAP_FLAG_NONE,&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(&UploadBuffer)));
    void* MapedData;
    UploadBuffer->Map(0,&CD3DX12_RANGE(0,Size), &MapedData);
    memcpy(MapedData, Data, Size);
    UploadBuffer->Unmap(0,&CD3DX12_RANGE(0,Size));

    RRHIBuffer* NewBuffer= new RRHIBuffer(Size/Stride,Stride,Size,DebugName);
    NewBuffer->mResource;

    ASSERT(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(Size,D3D12_RESOURCE_FLAG_NONE,0),
         D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&(NewBuffer->mResource))));

    return nullptr;
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
