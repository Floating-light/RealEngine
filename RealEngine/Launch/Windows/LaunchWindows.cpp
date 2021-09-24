#include <Windows.h>

extern int RealMain(const wchar_t* CmdLine);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char*, _In_ int nCmdShow)
{
    return RealMain(::GetCommandLineW());
}