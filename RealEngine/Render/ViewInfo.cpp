#include "ViewInfo.h"

void RViewInfo::AddPrimitive(const std::shared_ptr<RPrimitiveObject>& Primitive)
{
    if(Primitive)
    {
        Primitives.emplace_back(Primitive);
    }
}

void RViewInfo::SetPrimitives(const std::vector<std::shared_ptr<RPrimitiveObject>>& InPrimitives)
{
    Primitives = InPrimitives;
}

void RViewInfo::SetRenderWindow(const std::shared_ptr<RGenericWindow>& Window)
{
    if(Window)
    {
        RenderWindow = Window;
    }
}

std::vector<std::shared_ptr<RPrimitiveObject>> RViewInfo::GetPrimitives() const 
{
    return Primitives;
}

