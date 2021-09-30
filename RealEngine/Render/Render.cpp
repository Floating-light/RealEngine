#include "Render.h"

RRenderer& RRenderer::Get()
{
    static RRenderer Renderer;
    return Renderer;
}
