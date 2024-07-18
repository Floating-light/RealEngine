#include "GraphicViewport.h"

#define SWAP_CHAIN_BUFFER_COUNT 3

RGraphicViewport::RGraphicViewport()
{
}

void RGraphicViewport::Initialize(ID3D12Device* InDevice, IDXGIFactory4* Factory, ID3D12CommandQueue* CommandQueue, HWND WindowHandle) 
{
	assert(InDevice && Factory); 

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {}; 
	swapChainDesc.Width = m_ViewportSize.Width;
	swapChainDesc.Height = m_ViewportSize.Height;
	swapChainDesc.Format = m_SwapChaineFormat;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = m_SwapChainBufferCount;
	swapChainDesc.SampleDesc.Count = 1; // MSAA 多重采样抗锯齿，
	swapChainDesc.SampleDesc.Quality = 0;
	// DXGI_SCALING_STRETCH	不论如何都拉伸图像填满整个窗口
	// DXGI_SCALING_NONE 不进行任何缩放，当图片分辨率低于窗口分辨率时，未填满的区域将是黑色的
	// DXGI_SCALING_ASPECT_RATIO_STRETCH 保持宽高比缩放
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	// 决定BackBufffer如何与FrontBuffer交换
	// DXGI_SWAP_EFFECT_DISCARD	每次交换后，BackBuffer的内容不被保证（未定义），可能会被丢弃。
	// DXGI_SWAP_EFFECT_SEQUENTIAL 每次交换后，BackBuffer不会丢弃任何内容，可以基于这个，只渲染变化的部分画面。
	// 
	// 这两个使用翻转，也就是直接交换Front和Back Buffer指针，不像上面那样复制内容。
	// DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL 使用帧交换机制(FlipMode), 将BackBuffer翻转到FrontBuffer，Buffer上的内容还是它上一次的样子
	// DXGI_SWAP_EFFECT_FLIP_DISCARD 与上一个类似，但BackBuffer的内容可以在交换后丢弃，可以提供最好的性能
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;// 允许窗口模式，全屏模式切换

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChaineDesc = {};
	fsSwapChaineDesc.Windowed = true;

	ASSERT(Factory->CreateSwapChainForHwnd(CommandQueue, WindowHandle, &swapChainDesc, &fsSwapChaineDesc, nullptr, &m_SwapChain)); 

	//ASSERT(Factory->MakeWindowAssociation(WindowHandle, DXGI_MWA_NO_ALT_ENTER)); 

	// Create RT view heap
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descHeapDesc.NumDescriptors = 20;
	descHeapDesc.NodeMask = 1;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; 

	ASSERT(InDevice->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_rtViewHeap)));  
	D3D12_CPU_DESCRIPTOR_HANDLE viewHandleBegin = m_rtViewHeap->GetCPUDescriptorHandleForHeapStart();
	m_rtHandleSize = InDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// Create RT view 
	for (uint32_t i = 0; i < m_SwapChainBufferCount; i++)
	{
		// 每次调用这个方法获得到Resoruce，都会使其引用计数+1，当获取方不再使用时，应该及时调用Release释放。  
		ASSERT(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_DisplayPlane[i])));
		InDevice->CreateRenderTargetView(m_DisplayPlane[i].Get(), nullptr, D3D12_CPU_DESCRIPTOR_HANDLE(viewHandleBegin.ptr + i * m_rtHandleSize));
	}
}

void RGraphicViewport::Present() 
{
	m_SwapChain->Present(0, 0);
	m_CurrentBuffer = (m_CurrentBuffer + 1) % m_SwapChainBufferCount;
}

void RGraphicViewport::SetViewportSize(uint32_t InWidth, uint32_t InHeight) 
{
	m_ViewportSize.TopLeftX = 0;
	m_ViewportSize.TopLeftY = 0;
	m_ViewportSize.Width = InWidth;
	m_ViewportSize.Height = InHeight;
	m_ViewportSize.MinDepth = D3D12_MIN_DEPTH;
	m_ViewportSize.MaxDepth = D3D12_MAX_DEPTH;

	m_Scissor.left = 0;
	m_Scissor.top = 0;

	m_Scissor.right = InWidth;
	m_Scissor.bottom = InHeight;
}
