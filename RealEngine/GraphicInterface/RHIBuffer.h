#pragma once 

#include "RHIResource.h"

#include "D3D12ThirdPart.h"

class RRHIBuffer : public RRHIResource
{
public:
    RRHIBuffer()
        : RRHIResource("")
        , mResource(nullptr)
        {}
    RRHIBuffer(uint32_t ElementCount, uint32_t ElementSize, uint32_t BufferSize, std::string_view DebugName)
        : RRHIResource(DebugName)
        , mResource(nullptr)
        , mElementCount(ElementCount)
        , mElementSize(ElementSize)
        , mBufferSize(BufferSize)
    { }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mResource;
    uint32_t mElementCount;
    uint32_t mElementSize;
    uint32_t mBufferSize;

    friend class RCommandContext;
};

class RRHIUploadBuffer : public RRHIResource
{

};
