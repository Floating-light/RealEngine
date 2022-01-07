#pragma once 

#include "Logging.h"
#include "Vector.h"
#include "Rotator.h"
#include "Transform.h"

extern std::wstring ExecutableDir;

class RCoreGlobal
{
public:
    static bool IsRequireExit() ;
    static void SetRequireExit();
private:
    static bool bRequireExit;
};