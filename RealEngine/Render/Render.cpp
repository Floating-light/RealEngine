#include "Render.h"

#include "GraphicInterface.h"
#include "Logging.h"
RRenderer& RRenderer::Get()
{
    static RRenderer Renderer;
    return Renderer;
}

void RRenderer::Init()
{

}

void RRenderer::OnUpdate()
{

}

void RRenderer::DoRender(RViewInfo& ViewInfo)
{
    RLOG(INFO)<< "---------------->>> " << std::endl;

GGraphicInterface;    
}