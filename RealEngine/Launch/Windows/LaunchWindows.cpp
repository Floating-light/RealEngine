#include <Windows.h>
#include "WindowsMisc.h"
extern int RealMain(const wchar_t* CmdLine);

int WINAPI WinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char*, _In_ int nCmdShow)
{
    hInstance = _hInstance;
    return RealMain(::GetCommandLineW());
}