#include "GraphicInterface.h"
#include "Logging.h"

RGraphicInterface* GGraphicInterface = nullptr;

void RHIInit()
{
    if(GGraphicInterface = CreatePlatformRHI())
    {
        GGraphicInterface->InitRHI();
    }
    else
    {
        RLOG(Fatal, "Create graphic interface failed ");
    }
}
void RHIExit()
{
    
}

TRefCountPtr<RRHIBuffer> RGraphicInterface::CreateBuffer(const void *Data, uint32_t Size, uint32_t Stride, std::string_view DebugName)
{
    RRHIBuffer* NewBuffer = nullptr;
    
    return TRefCountPtr<RRHIBuffer>();
}
