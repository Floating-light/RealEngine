#include "Engine.h"

#include "Application.h"
#include "Render.h"
#include "GameViewportClient.h"

REngine* REngine::Get()
{
    static REngine* Engine = new REngine();
    return Engine;
}

void REngine::OnInit()
{
    RApplication::Create();
    
    RRenderer::Get().Init(RApplication::Get().GetMainWindow());

    GameViewportClient = std::shared_ptr<RGameViewportClient>(new RGameViewportClient());
    RApplication::Get().RegisterGameViewport(GameViewportClient);
}

void REngine::OnUpdate()
{
    RApplication::Get().ProcessInput();

    // update scene 

    // Render 
    RViewInfo ViewInfo;
    ViewInfo.RenderWindow = RApplication::Get().GetMainWindow();
    // RRenderer::Get().DoRender(ViewInfo);
}

void REngine::OnDestoryed()
{

}