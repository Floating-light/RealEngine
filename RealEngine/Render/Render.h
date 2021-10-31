#pragma once 

#include "ViewInfo.h"

class RRenderer
{
public:
    static RRenderer& Get();
    void Init(std::shared_ptr<RGenericWindow> Window);
    void DoRender(RViewInfo& ViewInfo);
private:
    
};