#include "../GenericPlatform/GenericPlatformFactory.h"

#include "WindowsApplication.h"
#include "WindowsMisc.h"
#include "WindowsPlatformDefine.h"
RGenericApplication * GenericPlatformMisc::CreatePlatformApplication()
{
    // https://stackoverflow.com/questions/23852864/loading-an-icon-in-windows-program
    HICON iconHandle = LoadIcon(hInstance, MAKEINTRESOURCE(REAL_ICON));
    return WindowsApplication::CreateWindowsApplication(hInstance, iconHandle);
}

void GenericPlatformMisc::PumpMessages()
{
    MSG msg;
    while(PeekMessage(&msg,nullptr, 0,0,PM_REMOVE ))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
