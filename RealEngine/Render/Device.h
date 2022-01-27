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
};