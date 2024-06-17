#pragma once 

#include "RHIResource.h"
class RRHIUploadBuffer;

class RRHIBuffer : public RRHIResource
{
public:
    virtual ~RRHIBuffer() { Destroy(); };
    void Create(const std::string& name, uint32_t NumElements, uint32_t ElementSize, const RRHIUploadBuffer& SrcData, uint32_t srcOffset = 0);
    void* Map();
    void Unmap(size_t begin = 0, size_t end = -1);
    size_t GetBufferSize() { return m_BufferSize; }
protected:
    RRHIBuffer() {};
    // 不同用途的Buffer需要不同的View
    virtual void CreateViewsDerived() = 0;
    size_t m_BufferSize;

    D3D12_RESOURCE_DESC CreateBufferDescribe();
    D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE HeapType);  

    D3D12_CPU_DESCRIPTOR_HANDLE m_UAV;
    D3D12_CPU_DESCRIPTOR_HANDLE m_SRV;
    uint32_t m_ElementCount;
    uint32_t m_ElementSize;
    D3D12_RESOURCE_FLAGS m_ResourceFlag;
};

class RRHIBufferByteAddress : RRHIBuffer
{
public:
    virtual void CreateViewsDerived() override;
};