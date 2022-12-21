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
private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mResource;
};

class RRHIUploadBuffer : public RRHIResource
{

};
