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
    RRHIBuffer(uint32_t ElementCount, uint32_t ElementSize, uint32_t BufferSize,D3D12_RESOURCE_STATES InState, std::string_view DebugName)
        : RRHIResource(DebugName)
        , mResource(nullptr)
        , mElementCount(ElementCount)
        , mElementSize(ElementSize)
        , mBufferSize(BufferSize)
        , mUsage(InState)
    { }
    ID3D12Resource* GetResource() const 
    {
        return mResource.Get();
    }
    
private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mResource;
    uint32_t mElementCount;
    uint32_t mElementSize;
    uint32_t mBufferSize;
    D3D12_RESOURCE_STATES mUsage;
    friend class RCommandContext;
};

class RRHIUploadBuffer : public RRHIResource
{

};
