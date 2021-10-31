#pragma once 

#include <memory>

class RSceneInfo;
class RGenericWindow;
struct RViewInfo
{
public:
    RSceneInfo* SceneInfor;
    std::shared_ptr<RGenericWindow> RenderWindow;
};