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