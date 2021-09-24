#include "RealMain.h"

int RealLuanch::Run(const wchar_t* Cmd)
{

}


int RealMain(const wchar_t* Cmd)
{
    return  RealLuanch().Run(Cmd);
}