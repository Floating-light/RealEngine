#include "Engine.h"

REngine* REngine::Get()
{
    static REngine* Engine = new REngine();
    return Engine;
}


void REngine::OnInit()
{
    
}
void REngine::OnUpdate()
{

}
void REngine::OnDestoryed()
{

}