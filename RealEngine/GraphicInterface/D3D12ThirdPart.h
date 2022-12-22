#pragma once 

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d3dx12.h>

#include "wrl.h"

#include "Core.h"

#define ASSERT(hr) \
    if(FAILED(hr)) \
    {\
        RLOG(Fatal, "HRESULT failed, hr = 0x%08X", hr); \
        __debugbreak();\
    }