#pragma once 

#include "ViewInfo.h"

class RRenderer
{
public:
    static RRenderer& Get();
    void Init();
    void OnUpdate();
    void DoRender(RViewInfo& ViewInfo);
};