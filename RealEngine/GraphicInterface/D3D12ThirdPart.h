#pragma once 

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d3dx12.h>
#include "d3dcompiler.h"

#include "wrl.h"

#include "Core.h"

#define D3D12_GPU_VIRTUAL_ADDRESS_NULL      ((D3D12_GPU_VIRTUAL_ADDRESS)0) 
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN   ((D3D12_GPU_VIRTUAL_ADDRESS)-1) 

#define ASSERTDX(hr) \
	{\
		if (FAILED(hr)) \
		{\
			RLOG(Fatal, "DirectX call failed at {}:{}. HRESULT = {:#010x}", \
								__FILE__, __LINE__, hr); \
			__debugbreak(); \
		}\
	}