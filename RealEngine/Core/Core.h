#pragma once 

#include "Logging.h"

extern std::wstring ExecutableDir;

class RCoreGlobal
{
public:
    static bool IsRequireExit() ;
    static void SetRequireExit();
private:
    static bool bRequireExit;
};