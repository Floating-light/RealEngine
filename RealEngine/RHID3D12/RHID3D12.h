#include "RHID3D12Private.h"

#include "GraphicInterface.h"


using Microsoft::WRL::ComPtr;

class RHID3D12Interface : public RGraphicInterface
{
public:

    virtual void InitRHI() = 0;
	virtual IGraphicViewport* CreateViewport(void* handle, int width, int height) = 0;

private:
    ComPtr<IDXGIFactory4> m_factory;
};