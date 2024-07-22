#pragma once 
#include <string_view>
#include <cassert>
#include "D3D12ThirdPart.h"

class RRHIResource
{
    friend class RCommandContext; 
public:
	RRHIResource() 
        : m_Resource(nullptr)
        , m_UsageState(D3D12_RESOURCE_STATE_COMMON)
        , m_GpuVirtualAddress(D3D12_GPU_VIRTUAL_ADDRESS_NULL)
    {}
    RRHIResource(ID3D12Resource* InResource, D3D12_RESOURCE_STATES InState)
        : m_Resource(InResource)
        , m_UsageState(InState)
        , m_GpuVirtualAddress(D3D12_GPU_VIRTUAL_ADDRESS_NULL)
    {}
    ~RRHIResource()
    {
        Destroy(); 
    }
    virtual void Destroy() 
    {
        m_Resource = nullptr;
        m_GpuVirtualAddress = D3D12_GPU_VIRTUAL_ADDRESS_NULL;
    }
    ID3D12Resource* GetResource() const { return m_Resource.Get(); }
    D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return m_GpuVirtualAddress; }
protected:
    Microsoft::WRL::ComPtr<ID3D12Resource> m_Resource;
    D3D12_RESOURCE_STATES m_UsageState;
    // https://learn.microsoft.com/en-us/windows/win32/direct3d12/using-resource-barriers-to-synchronize-resource-states-in-direct3d-12#split-barriers
    // https://learn.microsoft.com/en-us/windows/win32/direct3d12/using-resource-barriers-to-synchronize-resource-states-in-direct3d-12#example-of-split-barriers
    D3D12_RESOURCE_STATES m_TransitioningState;
    D3D12_GPU_VIRTUAL_ADDRESS m_GpuVirtualAddress;
}; 

