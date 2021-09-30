#include "Core.h"

bool RCoreGlobal::bRequireExit = false;

bool RCoreGlobal::IsRequireExit() 
{
    return RCoreGlobal::bRequireExit;
}

void RCoreGlobal::SetRequireExit()
{
    RCoreGlobal::bRequireExit = true;
}
