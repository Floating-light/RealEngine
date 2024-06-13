#include "Render.h"

#include "GraphicInterface.h"
#include "Logging.h"

#include "D3DApp.h"
// for test purpose
#include "GenericPlatform/GenericWindow.h"
RRenderer& RRenderer::Get()
{
    static RRenderer Renderer;
    return Renderer;
}
static D3DApp* TestApp = nullptr;
void RRenderer::Init(std::shared_ptr<RGenericWindow> Window)
{
    TestApp = new D3DApp();
    TestApp->InitializeViewport(Window->GetWindowHandle(),1280, 720,L"Test");
    TestApp->Setup();
}

void RRenderer::DoRender(RViewInfo& ViewInfo)
{
    // RLOG(INFO)<< "---------------->>> " << std::endl;
    std::vector<std::shared_ptr<RPrimitiveObject>> LocalPrimitives = ViewInfo.GetPrimitives();

    TestApp->OnUpdate(0.1);
    TestApp->OnRender();
}