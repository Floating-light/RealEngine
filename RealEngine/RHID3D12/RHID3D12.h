#include "RHID3D12Private.h"
#include "GraphicInterface.h"

#include <memory>

// ZMACRO_MSVCStaticLib_Reference(RHID3D12)
using Microsoft::WRL::ComPtr;
class RD3D12Adapter;
class RHID3D12 : public RGraphicInterface
{
public:

    virtual void InitRHI() ;
	virtual IGraphicViewport* CreateViewport(void* handle, int width, int height) {return nullptr;};

private:
    std::shared_ptr<RD3D12Adapter> m_adapter;
};