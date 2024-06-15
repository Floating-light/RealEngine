#pragma once 
#include "D3D12ThirdPart.h"

using Microsoft::WRL::ComPtr; 
class RGraphicViewport
{
public:
	RGraphicViewport(ID3D12Device* InDevice);

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain = nullptr; 
};