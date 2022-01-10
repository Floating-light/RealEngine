#pragma once 

class GenericPlatformMisc
{
public:
    static class RGenericApplication* CreatePlatformApplication();
    static void PumpMessages();
};