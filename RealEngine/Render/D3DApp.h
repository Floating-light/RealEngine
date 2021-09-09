#pragma once 
#include "stdafx.h"
#include <stdexcept>

using Microsoft::WRL::ComPtr;
// D3D12 : manage the resource in gpu memory manually(residency)
// D3D11 : manage by system

class D3DApp
{
public:
    void Setup() ;
    void LoadAsset();
    // A GPU(adapter) connect to multiple monitor(display output )
    // IDXGIAdapter display adapter 
    void LogAdapters() ;
    // IDXGIOutput display output(deveice) associate with video card
    void LogAdapterOutputs(ComPtr<IDXGIAdapter> adapter, const std::wstring& desctiption);
    void LogOutputDisplayModels(ComPtr<IDXGIOutput> output, DXGI_FORMAT format);
    void WaitForPreviousFrame();
    static const int SwapChainBufferCount = 2;
protected:
    void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter = false);
    void CreateSwapChain();
private:
    ComPtr<IDXGISwapChain> m_swapChain;
    HWND m_hMainWnd;
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

    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValue;
    HANDLE m_fenceEvent;

    UINT m_rtvDescHeapIncSize;
    UINT m_dsvDescHeapIncSize;

    int m_currentBackBuffer;
};

// concept : 
// barrier : https://docs.microsoft.com/en-us/windows/win32/direct3d12/using-resource-barriers-to-synchronize-resource-states-in-direct3d-12
