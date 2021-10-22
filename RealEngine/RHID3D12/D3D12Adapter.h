#pragma once 
#include "RHID3D12Private.h"

using Microsoft::WRL::ComPtr;

class RD3D12Adapter
{
public:
    RD3D12Adapter(ComPtr<IDXGIAdapter> adapter);
    
private:
    ComPtr<IDXGIAdapter> m_Adapter;
};
