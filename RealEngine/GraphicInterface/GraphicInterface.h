	#pragma once 

#include <unordered_map>
#include <functional>

#include "GraphicViewport.h"
#include "Module/ModuleManager.h"
#include "RHIBuffer.h"
#include "RHIResource.h"
#include "RefCounting.h"
class RCommandContext;
class RCommandContextManger; 
class RCommandListManager; 
class RGraphicViewport;
class RAdapter;

class RGraphicInterface
{
public:
	
    virtual void InitRHI() ;
	virtual void DeInitRHI();
	virtual void InitilizeViewport(HWND WindowHandle, uint32_t Width, uint32_t Height) ;
	void Present();
	RGraphicViewport* GetViewport()const { return Viewport.get(); } 
	
	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice()const {return m_Device;}

	virtual TRefCountPtr<RRHIBuffer> CreateBuffer(const void* Data, uint32_t Size, uint32_t Stride, std::string_view DebugName);


	RCommandContext* BeginCommandContext(const std::string& ID); 
	RCommandListManager* GetCommandListManager()const {return CommandListManager.get();}
	RCommandContextManger* GetCommandContextManger()const { return ContextManager.get(); }
private:
	std::unique_ptr<RCommandContextManger> ContextManager;
	std::unique_ptr<RCommandListManager> CommandListManager;
	std::unique_ptr<RGraphicViewport> Viewport = nullptr;
	std::shared_ptr<RAdapter> mAdapter;
	Microsoft::WRL::ComPtr<ID3D12Device> m_Device = nullptr; 
	Microsoft::WRL::ComPtr<IDXGIFactory4> m_Factory4 = nullptr;
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
