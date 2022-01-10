#pragma once 

#include "ViewInfo.h"

class RRenderer
{
public:
    static RRenderer& Get();
    
    void Init(std::shared_ptr<RGenericWindow> Window);
    
    // Called from engine every frame to rendering scene
    void DoRender(RViewInfo& ViewInfo);
private:
    
};