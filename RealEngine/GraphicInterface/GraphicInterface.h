	#pragma once 

#include <unordered_map>
#include <functional>

#include "GraphicViewport.h"
#include "Module/ModuleManager.h"

class RGraphicInterface
{
public:
    virtual void InitRHI() = 0;
	virtual IGraphicViewport* CreateViewport(void* handle, int width, int height) = 0;
	
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
