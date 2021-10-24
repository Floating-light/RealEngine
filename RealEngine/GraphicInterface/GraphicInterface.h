	#pragma once 

#include <unordered_map>
#include <functional>

#include "GraphicViewport.h"


class RGraphicInterface
{
public:
    virtual void InitRHI() = 0;
	virtual IGraphicViewport* CreateViewport(void* handle, int width, int height) = 0;

	static std::unordered_map<std::wstring, std::function<RGraphicInterface* ()>>  m_Factory;
};

// https://stackoverflow.com/questions/9459980/c-global-variable-not-initialized-when-linked-through-static-libraries-but-ok
// https://stackoverflow.com/questions/9459980/c-global-variable-not-initialized-when-linked-through-static-libraries-but-ok/11336506#11336506
// https://github.com/zoolib/zoolib_cxx/blob/6e52b8ac85c6636d70f14b4c8123ff67e5dc5837/Core/zoolib/Compat_MSVCStaticLib.h
// https://stackoverflow.com/questions/2991927/how-to-force-inclusion-of-an-object-file-in-a-static-library-when-linking-into-e
template<class InterfaceImplClass>
struct RHIInterfaceRegistrant
{
	RHIInterfaceRegistrant(const std::wstring& InterfaceName)
	{
		RGraphicInterface::m_Factory.emplace(InterfaceName, []()
		{
			return new InterfaceImplClass;
		});
	}
	RGraphicInterface* Initlialize()
	{
		return nullptr;
	}

};
extern RGraphicInterface* GGraphicInterface;
extern RGraphicInterface* CreatePlatformRHI();
extern void RHIInit();
extern void RHIExit();
