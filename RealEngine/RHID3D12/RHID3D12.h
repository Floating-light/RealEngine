#include "RHID3D12Private.h"

#include "GraphicInterface.h"


using Microsoft::WRL::ComPtr;

class RHID3D12 : public RGraphicInterface
{
public:

    virtual void InitRHI() ;
	virtual IGraphicViewport* CreateViewport(void* handle, int width, int height) ;

private:
    ComPtr<IDXGIFactory4> m_factory;
};