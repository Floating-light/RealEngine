#pragma once 
#include "D3D12ThirdPart.h"

class RDevice
{

private:
    Microsoft::WRL::ComPtr<ID3D12Device> mDevice;
};