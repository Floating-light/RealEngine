#include "ViewInfo.h"

void RViewInfo::AddPrimitive(const std::shared_ptr<RPrimitiveObject>& Primitive)
{
    if(Primitive)
    {
        Primitives.emplace_back(Primitive);
    }
}

void RViewInfo::SetRenderWindow(const std::shared_ptr<RGenericWindow>& Window)
{
    if(Window)
    {
        RenderWindow = Window;
    }
}
