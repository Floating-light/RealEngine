#include "Engine.h"

#include <filesystem>

#include "Application.h"
#include "Render.h"
#include "GameViewportClient.h"
#include "PrimitiveInfo.h"
#include "Vector4D.h"
#include "AssetImporter.h"
#include "ModelData.h"
REngine* REngine::Get()
{
    static REngine* Engine = new REngine();
    return Engine;
}
static std::vector<std::shared_ptr<RPrimitiveObject>> Objects;

void REngine::OnInit()
{
    // 创建Windows
    RApplication::Create();
    
    // 初始化渲染器
    RRenderer::Get().Init(RApplication::Get().GetMainWindow());

    // 向App注册ViewportClinet，以便ViewportClinet可以响应AppUI相关事件
    GameViewportClient = std::shared_ptr<RGameViewportClient>(new RGameViewportClient());
    RApplication::Get().RegisterGameViewport(GameViewportClient);
    
    // 加载渲染场景
    // E:\Workspace\RealEngine\build\bin\Debug\
    // E:\Workspace\RealEngine\build\RealEngine\Launch\
    
    std::filesystem::path FilePath = std::filesystem::absolute("../../resources/HuangQuan/星穹铁道—黄泉（轴修复）.pmx");
    RCHECK(std::filesystem::exists(FilePath));
    std::shared_ptr<RModelData> ModelData = RAssetImporter::ImportModelNew(FilePath.string()); 
    std::shared_ptr<RPrimitiveObject> Obj(new RPrimitiveObject());
    Obj->SetModelData(ModelData, "Model1",RTransform(Vector(0.f, 0.f, 0.f)));
    //RaiDenShougunObj->IntializeBuffer();
    Objects.push_back(Obj);

    std::shared_ptr<RModelData> PlaneData = RAssetImporter::MakePlane(); 
    PlaneData->SetMaterials({ ModelData->GetMaterial(2) });
    std::shared_ptr<RPrimitiveObject> PlanePrimtive(new RPrimitiveObject()); 
    RTransform t(Vector(10.f, 20.f, 0.f));
    t.Scale = Vector(4.f, 2.f, 2.f);
    PlanePrimtive->SetModelData(PlaneData, "Plane1", t); 

    Objects.push_back(PlanePrimtive);
}

void REngine::OnUpdate()
{
    // App 更新，处理UI输入
    RApplication::Get().ProcessInput();

    // update scene 
    GameViewportClient->Update();

    // init scene view infor
    RViewInfo ViewInfo;
    ViewInfo.SetRenderWindow(RApplication::Get().GetMainWindow());
    ViewInfo.SetPrimitives(Objects);
    GameViewportClient->SetUpView(ViewInfo);
    
    // render scene
    RRenderer::Get().DoRender(ViewInfo);
}

void REngine::Destoryed()
{
    RRenderer::Get().Destroy();
    RLOG(Info, "{}: Engine destory", __FUNCTION__); 
}