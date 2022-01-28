#pragma once 
#include "stdafx.h"

using Microsoft::WRL::ComPtr;

class RDevice
{
    RDevice();
private:
    ComPtr<IDXGIFactory4> mFactory;
    ComPtr<ID3D12Device> mDevice;
    ComPtr<IDXGIAdapter1> mAdapter; 
    
    // the size of the handle increment of the descriptor heap for the render-target view 
    unsigned int m_rtvDescriptorSize;
    // the descriptor handle increment of depth-stencil view(descriptor) 
    unsigned int m_dsvDescriptorSize;
    // the descriptor handle increment of the descriptor heap for the combination of constant-buffer, shader-resource, and unordered-access views .
    unsigned int m_cbvUavDescriptorSize ;
};