#pragma once 
#include "stdafx.h"
#include <stdexcept>

using Microsoft::WRL::ComPtr;
// D3D12 : manage the resource in gpu memory manually(residency)
// D3D11 : manage by system

class D3DApp
{
public:
    D3DApp(UINT width, UINT height, const std::wstring& title);
    void Setup() ;
    void LoadAsset();
    void OnUpdate(){};
    void OnRender(){};
    void OnKeyDown(UINT8 key){};
    void OnKeyUp(UINT8 key){};

    // A GPU(adapter) connect to multiple monitor(display output )
    // IDXGIAdapter display adapter 
    void LogAdapters() ;
    // IDXGIOutput display output(deveice) associate with video card
    void LogAdapterOutputs(ComPtr<IDXGIAdapter> adapter, const std::wstring& desctiption);
    void LogOutputDisplayModels(ComPtr<IDXGIOutput> output, DXGI_FORMAT format);
    void WaitForPreviousFrame();
    UINT GetWidth() const {return m_clientWidth;};
    UINT GetHeight() const {return m_clientHeight;};
    const WCHAR* GetTitle() const { return m_title.c_str();}
    static const int SwapChainBufferCount = 2;
protected:
    void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter = false);
    void CreateSwapChain();
private:
    UINT m_clientWidth, m_clientHeight;
    std::wstring m_title;
    ComPtr<IDXGISwapChain3> m_swapChain;

    UINT m_rtvDescriptorSize;
    UINT m_dsvDescriptorSize;
    UINT m_cbvUavDescriptorSize;
    DXGI_FORMAT m_backBufferFormat;
    DXGI_FORMAT m_depthStencilFormat;
    
    BOOL m_4xMsaaState;
    UINT8 m_4xMassQuality;
    int m_currentBackBuffer;
    Microsoft::WRL::ComPtr<IDXGIFactory4> m_factory;
    ComPtr<ID3D12Device> m_device;
    // the container of CommandList for GPU to execute.
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    // the actually place to store command 
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    ComPtr<ID3D12GraphicsCommandList> m_commandList;

    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    ComPtr<ID3D12DescriptorHeap> m_dsvHeap;

    ComPtr<ID3D12Resource> m_renderTargets[SwapChainBufferCount];
    ComPtr<ID3D12Resource> m_depthStencilBuffer;
    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValue;
    HANDLE m_fenceEvent;
};

// concept : 
// barrier : https://docs.microsoft.com/en-us/windows/win32/direct3d12/using-resource-barriers-to-synchronize-resource-states-in-direct3d-12
