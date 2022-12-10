#pragma once 

#include <cassert>

#include "Logging.h"
#include "Vector.h"
#include "Rotator.h"
#include "Transform.h"
#include "Matrix4.h"
#include "Vector2D.h"

extern std::wstring ExecutableDir;

class RCoreGlobal
{
public:
    static bool IsRequireExit() ;
    static void SetRequireExit();
private:
    static bool bRequireExit;
};