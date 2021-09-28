#pragma once

#include <windows.h>
#include "../GenericPlatform/GenericApplication.h"

class WindowsApplication : public RGenericApplication
{
public:
    static WindowsApplication* CreateWindowsApplication(const HINSTANCE hInstance, const HICON IconHandle) ;
    virtual std::shared_ptr<class RGenericWindow> MakeWindow() override;    
};