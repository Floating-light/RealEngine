#pragma once 

class RRenderer
{
public:
    static RRenderer& Get();
    void Init();
    void OnUpdate();
    void DoRender(class RSceneInfo* SceneInfor, class RTarget* RenderTarget);
};