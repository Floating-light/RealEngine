#include "CommandContext.h"

RRHIBuffer *RCommandContext::CreateBuffer(const void *Data, uint32_t Size, uint32_t Stride, std::string_view DebugName)
{
    mCommandList;
    D3D12_HEAP_PROPERTIES UploadHeapProps = GetUploadBufferHeapProps();
    D3D12_RESOURCE_DESC ResourceDesc = GetUploadBufferResourceDesc(Size);
    
    Microsoft::WRL::ComPtr<ID3D12Resource> UploadBuffer;
    mDevice->CreateCommittedResource(&UploadHeapProps,D3D12_HEAP_FLAG_NONE,&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(&UploadBuffer));
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
