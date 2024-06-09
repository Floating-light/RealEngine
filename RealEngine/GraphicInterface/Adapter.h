#pragma once 
#include "D3D12ThirdPart.h"
#include "RefCounting.h"

class RAdapter
{
public:
    RAdapter(TRefCountPtr<IDXGIAdapter> Adapter);
    void InitializeDevice();
private:
    TRefCountPtr<IDXGIFactory7> mFactory;
    TRefCountPtr<ID3D12Device9> mDevice;
    TRefCountPtr<IDXGIAdapter4> mAdapter;
};