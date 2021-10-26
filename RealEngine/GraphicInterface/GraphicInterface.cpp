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
        RLOG(FATAL) << "Create graphic interface failed " << std::endl;
    }
}
void RHIExit()
{
    
}