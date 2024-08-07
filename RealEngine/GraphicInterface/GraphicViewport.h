#pragma once 
#include "D3D12ThirdPart.h"

class RGraphicViewport
{
	template <typename T> 
	using ComPtr = Microsoft::WRL::ComPtr<T>;   
public:
	RGraphicViewport();
	~RGraphicViewport() {};

	void Initialize(ID3D12Device* InDevice, IDXGIFactory4* Factory, ID3D12CommandQueue* CommandQueue, HWND WindowHandle);

	void Present();

	void SetViewportSize(uint32_t InWidth, uint32_t InHeight);
	DXGI_FORMAT GetSwapChainFormat() const { return m_SwapChaineFormat; }
	ID3D12Resource* GetCurrentRT()const { return m_DisplayPlane[m_CurrentBuffer].Get(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTViewHandle()const 
	{ return D3D12_CPU_DESCRIPTOR_HANDLE{ m_rtViewHeap->GetCPUDescriptorHandleForHeapStart().ptr + m_CurrentBuffer * m_rtHandleSize }; } 
	D3D12_VIEWPORT GetViewportSize()const { return m_ViewportSize; };
	D3D12_RECT GetScissor()const { return m_Scissor; }
private:
	ComPtr<IDXGISwapChain1> m_SwapChain = nullptr;  
	D3D12_VIEWPORT m_ViewportSize;
	D3D12_RECT m_Scissor;

	DXGI_FORMAT m_SwapChaineFormat = DXGI_FORMAT_R10G10B10A2_UNORM; 
	static constexpr const uint8_t m_SwapChainBufferCount = 3; 
	ComPtr<ID3D12DescriptorHeap> m_rtViewHeap;
	ComPtr<ID3D12Resource> m_DisplayPlane[m_SwapChainBufferCount]; 
	uint8_t m_CurrentBuffer = 0;
	uint32_t m_rtHandleSize;
};