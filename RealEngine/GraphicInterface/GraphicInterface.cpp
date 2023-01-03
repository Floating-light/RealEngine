#include "GraphicInterface.h"
#include "Logging.h"
#include "CommandContext.h"

RGraphicInterface* GGraphicInterface = nullptr;

void RHIInit()
{
    if(GGraphicInterface = CreatePlatformRHI())
    {
        GGraphicInterface->InitRHI();
    }
    else
    {
        // RLOG(Fatal, "Create graphic interface failed ");
        // 暂时全部实现在这个模块
    }
}
void RHIExit()
{
    
}

TRefCountPtr<RRHIBuffer> RGraphicInterface::CreateBuffer(const void *Data, uint32_t Size, uint32_t Stride, std::string_view DebugName)
{
    return mCommandContext->CreateBuffer(Data, Size, Stride,DebugName);
}
