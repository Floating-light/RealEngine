#pragma once 

#include "Logging.h"

class RCoreGlobal
{
public:
    static bool IsRequireExit() ;
    static void SetRequireExit();
private:
    static bool bRequireExit;
};