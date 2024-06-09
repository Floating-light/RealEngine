	#pragma once 

#include <unordered_map>
#include <functional>

#include "GraphicViewport.h"
#include "Module/ModuleManager.h"
#include "RHIBuffer.h"
#include "RHIResource.h"
#include "RefCounting.h"
class RCommandContext;
class RAdapter;
class RGraphicInterface
{
public:
    virtual void InitRHI() ;
	virtual IGraphicViewport* CreateViewport(void* handle, int width, int height) {return nullptr;};
	virtual TRefCountPtr<RRHIBuffer> CreateBuffer(const void* Data, uint32_t Size, uint32_t Stride, std::string_view DebugName);
private:
	RCommandContext* mCommandContext;
	std::shared_ptr<RAdapter> mAdapter;
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
