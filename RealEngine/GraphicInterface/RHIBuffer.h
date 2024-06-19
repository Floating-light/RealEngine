#pragma once 

#include "RHIResource.h"
class RRHIUploadBuffer;

class RRHIBuffer : public RRHIResource
{
public:
    virtual ~RRHIBuffer() { Destroy(); };
    void Create(const std::string& Name, uint32_t NumElements, uint32_t ElementSize, const RRHIUploadBuffer& SrcData, uint32_t srcOffset = 0);
    void Create(const std::string& Name, uint32_t NumElements, uint32_t ElementSize, const void* InitalData);
    size_t GetBufferSize()const { return m_BufferSize; }
    D3D12_CPU_DESCRIPTOR_HANDLE GetSRV() const { return m_SRV; }
    D3D12_CPU_DESCRIPTOR_HANDLE GetUAV() const { return m_UAV; }
protected:
    RRHIBuffer()
        : RRHIResource()
        , m_BufferSize(0)
        , m_ElementCount(0)
        , m_ElementSize(0)
    {
        m_UAV.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
        m_SRV.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
        m_ResourceFlag = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    };
    // 不同用途的Buffer需要不同的View
    virtual void CreateViewsDerived() = 0;
    D3D12_RESOURCE_DESC CreateBufferDescribe();

    size_t m_BufferSize; 
    uint32_t m_ElementCount;
    uint32_t m_ElementSize;

    D3D12_CPU_DESCRIPTOR_HANDLE m_UAV; // 多次创建可以复用
    D3D12_CPU_DESCRIPTOR_HANDLE m_SRV;
    D3D12_RESOURCE_FLAGS m_ResourceFlag;
};

class RRHIBufferByteAddress : public RRHIBuffer
{
public:
    virtual void CreateViewsDerived() override;
};