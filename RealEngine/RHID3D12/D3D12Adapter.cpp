#include "D3D12Adapter.h"

RD3D12Adapter::RD3D12Adapter(ComPtr<IDXGIAdapter> adapter)
: m_Adapter(adapter)
{

};
