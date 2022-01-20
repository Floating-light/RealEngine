#include "Engine.h"

#include "Application.h"
#include "Render.h"
#include "GameViewportClient.h"
#include "PrimitiveInfo.h"
#include "Vector4D.h"

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
    RVector4D V4(1.f,1.f,1.f,1.f);
    RApplication::Get().SetOnMainWindowClosed(std::bind(&REngine::OnDestoryed, this));
}

void REngine::OnUpdate()
{
    RApplication::Get().ProcessInput();

    // update scene 

    // Render 
    RViewInfo ViewInfo;
    ViewInfo.SetRenderWindow(RApplication::Get().GetMainWindow());
    
    // ViewInfo contain the primitive information to rendering .
    RRenderer::Get().DoRender(ViewInfo);
}

void REngine::OnDestoryed()
{
    RLOG(INFO) << __FUNCTION__ << "Engine destory " ;
}