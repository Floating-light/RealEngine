#pragma once 

#include "RHIResource.h"

class RRHIUploadBuffer : public RRHIResource
{
public:
    virtual ~RRHIUploadBuffer() { Destroy(); }; 
    void Create(const std::string& name, size_t BufferSize);
    void* Map();
    void Unmap(size_t begin = 0, size_t end = -1);
    size_t GetBufferSize() const { return m_BufferSize; }
protected:
    size_t m_BufferSize;
};
