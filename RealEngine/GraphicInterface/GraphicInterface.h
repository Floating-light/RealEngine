#include <unordered_map>
#include <functional>

#include "GraphicViewport.h"


class RGraphicInterface
{
public:
    virtual void InitRHI() = 0;
	virtual IGraphicViewport* CreateViewport(void* handle, int width, int height) = 0;

	static std::unordered_map<std::string, std::function<RGraphicInterface* ()>>  m_Factory;
};
