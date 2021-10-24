#include "Core.h"

std::wstring ExecutableDir = L"";


bool RCoreGlobal::bRequireExit = false;

bool RCoreGlobal::IsRequireExit() 
{
    return RCoreGlobal::bRequireExit;
}

void RCoreGlobal::SetRequireExit()
{
    RCoreGlobal::bRequireExit = true;
}
