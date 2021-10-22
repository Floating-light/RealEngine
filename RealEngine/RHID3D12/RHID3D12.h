#include "RHID3D12Private.h"
#include "GraphicInterface.h"

#include <memory>

#if not defined(ZMACRO_MSVCStaticLib_Reference)
	#define ZMACRO_MSVCStaticLib_Reference(distinguisher_p) \
		namespace ZooLib { \
		namespace MSVCStaticLib { \
		namespace distinguisher_p { \
		void sDummyFunction(); \
		static struct DummyClass \
			{ DummyClass() { sDummyFunction();} } sDummyClass; \
		} /* namespace distinguisher_p */ \
		} /* namespace MSVCStaticLib */ \
		} /* namespace ZooLib */
#endif

#if not defined(ZMACRO_MSVCStaticLib_cpp)
	#define ZMACRO_MSVCStaticLib_cpp(distinguisher_p) \
		namespace ZooLib { \
		namespace MSVCStaticLib { \
		namespace distinguisher_p { \
		void sDummyFunction() {} \
		} /* namespace distinguisher_p */ \
		} /* namespace MSVCStaticLib */ \
		} /* namespace ZooLib */
#endif


ZMACRO_MSVCStaticLib_Reference(RHID3D12)
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