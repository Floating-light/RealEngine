	#pragma once 

#include <unordered_map>
#include <functional>
#include <array>

#include "D3D12ThirdPart.h"
#include "Module/ModuleManager.h"

#include "DescriptorHeap.h"
#include "LinearAllocator.h"

class RCommandContext;
class RCommandContextManger; 
class RCommandListManager; 
class RGraphicViewport;
class RAdapter;
class RRHIBuffer;
class RRHIUploadBuffer;

class RGraphicInterface
{
public:
	
    virtual void InitRHI() ;
	virtual void DeInitRHI();
	virtual void InitilizeViewport(HWND WindowHandle, uint32_t Width, uint32_t Height) ;
	void Present();
	RGraphicViewport* GetViewport()const { return Viewport.get(); } 
	
	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice()const {return m_Device;}
	ID3D12Device* GetDeviceRaw()const {return m_Device.Get();} 

	RCommandContext* BeginCommandContext(const std::string& ID); 
	RCommandListManager* GetCommandListManager()const {return CommandListManager.get();}
	RCommandContextManger* GetCommandContextManger()const { return ContextManager.get(); }
	RLinearAllocatorPageManager* GetLinearAllocatorPageMananger(ELinearAllocatorType Type){ return (m_LinearAllocatorManager[Type].get()); }
	void InitializeBuffer(RRHIBuffer& DestBuffer, const RRHIUploadBuffer& SrcBuffer, size_t SrcOffset, size_t NumBytes = -1, size_t DestOffset = 0);

	D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE HeapType);

private:
	std::unique_ptr<RCommandContextManger> ContextManager;
	std::unique_ptr<RCommandListManager> CommandListManager;
	std::unique_ptr<RGraphicViewport> Viewport = nullptr;
	std::shared_ptr<RAdapter> mAdapter;
	Microsoft::WRL::ComPtr<ID3D12Device> m_Device = nullptr; 
	Microsoft::WRL::ComPtr<IDXGIFactory4> m_Factory4 = nullptr;
	std::vector<RDescriptorAllocator> m_DescriptorAllocator;  
	std::array<std::unique_ptr<RLinearAllocatorPageManager>, ELinearAllocatorType::kNumAllocatorTypes> m_LinearAllocatorManager;  
};

class GraphicModuleBase : public IModuleInterface
{
	public:
	virtual RGraphicInterface* CreateRHI() = 0;
};

extern RGraphicInterface* GGraphicInterface;

// implementation in platform file
extern RGraphicInterface* CreatePlatformRHI();
extern void RHIInit();
extern void RHIExit();
