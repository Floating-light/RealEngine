#include "../GenericPlatform/GenericPlatformFactory.h"

#include "WindowsApplication.h"
#include "WindowsMisc.h"

GenericApplication * GenericPlatformFactory::CreatePlatformApplication()
{
    // https://stackoverflow.com/questions/23852864/loading-an-icon-in-windows-program
    HICON iconHandle = LoadIcon(NULL, IDI_APPLICATION);
    return WindowsApplication::CreateWindowsApplication(hInstance, iconHandle);
}