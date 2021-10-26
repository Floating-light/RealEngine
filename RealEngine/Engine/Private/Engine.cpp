#include "Engine.h"

#include "Application.h"
#include "Render.h"
REngine* REngine::Get()
{
    static REngine* Engine = new REngine();
    return Engine;
}


void REngine::OnInit()
{
    
    RApplication::Get()->Initilization();
}
void REngine::OnUpdate()
{
    RApplication::Get()->ProcessInput();

    // update scene 

    // Render 
    RViewInfo ViewInfo;
    RRenderer::Get().DoRender(ViewInfo);
}
void REngine::OnDestoryed()
{

}