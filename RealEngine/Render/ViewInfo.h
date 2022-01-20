#pragma once 

#include <memory>
#include <vector>

class RSceneInfo;
class RGenericWindow;
class RPrimitiveObject;

struct RViewInfo
{
public:
    void AddPrimitive(const std::shared_ptr<RPrimitiveObject>& Primitive);
    void SetRenderWindow(const std::shared_ptr<RGenericWindow>& Window);
private:
    RSceneInfo* SceneInfor;
    std::shared_ptr<RGenericWindow> RenderWindow;
    std::vector<std::shared_ptr<RPrimitiveObject>> Primitives;
};