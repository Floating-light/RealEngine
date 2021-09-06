#include "D3DApp.h"
#include "Helper.h"
#include <iostream>
#include <vector>
void D3DApp::Setup()
{
    UINT dxgiFactoryFlags = 0;
    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_factory)));
}
void D3DApp::LogAdapters() const
{
    UINT i = 0;
    IDXGIAdapter* adapter  = nullptr;
    std::vector<IDXGIAdapter> adapterList;
    while(m_factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc(&desc);

        std::wstring text = L"***Adapter: ";
        text += desc.Description ;
        text += L"\n";

        OutputDebugString(text.c_str());

        ++i;
    }

    std::cout << "LogAdapters ... " << std::endl;
}