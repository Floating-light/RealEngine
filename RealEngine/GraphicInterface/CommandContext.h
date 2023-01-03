#pragma once 
#include <string_view>

#include "D3D12ThirdPart.h"
#include "RefCounting.h"

class RRHIBuffer;
class RCommandContext
{
public:
    RRHIBuffer* CreateBuffer(const void *Data, uint32_t Size, uint32_t Stride, std::string_view DebugName);
private:
    D3D12_HEAP_PROPERTIES GetUploadBufferHeapProps() const;
    D3D12_RESOURCE_DESC GetUploadBufferResourceDesc(uint32_t BufferSize) const;

    // Resource
    void TransitionResource(RRHIBuffer* Buffer, D3D12_RESOURCE_STATES NewState, bool FlushImmediate);

private:
    ID3D12GraphicsCommandList* mCommandList;
    ID3D12Device* mDevice;
};