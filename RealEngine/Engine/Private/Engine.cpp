#include "Engine.h"

#include "Application.h"

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
}
void REngine::OnDestoryed()
{

}