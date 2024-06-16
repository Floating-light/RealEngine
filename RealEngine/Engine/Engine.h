#pragma once 
#include <memory>

class RGameViewportClient;

class REngine
{
public:
    static REngine* Get();
    void OnInit();
    void OnUpdate();
    void Destoryed();
private:
    std::shared_ptr<RGameViewportClient> GameViewportClient;
};
