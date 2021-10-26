#include <Windows.h>
#include "WindowsMisc.h"
extern int RealMain(const wchar_t* CmdLine);

int WINAPI WinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char*, _In_ int nCmdShow)
{
    hInstance = _hInstance;

    unsigned char * lpSrc = new unsigned char(5);
    *(lpSrc) = (unsigned char )(16 + *lpSrc);
    *(lpSrc) = (unsigned char )(16 + *lpSrc);
    __int8;
    typedef int8 __int8;
    int8;int8_t;
    return RealMain(::GetCommandLineW());
}