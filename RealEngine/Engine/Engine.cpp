#include "Engine.h"

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
    
    RRenderer::Get().Init(RApplication::Get().GetMainWindow());

    GameViewportClient = std::shared_ptr<RGameViewportClient>(new RGameViewportClient());
    RApplication::Get().RegisterGameViewport(GameViewportClient);
    
    //RApplication::Get().SetOnMainWindowClosed(std::bind(&REngine::Destoryed, this)); 
    //std::shared_ptr<RPrimitiveObject>  Obj1 = std::make_shared<RPrimitiveObject>();
    //Obj1->mVertexData = 
    //{
    //    RVertex{Vector3D(-1.0f, -1.0f, -1.0f),  RColor::White},
    //    RVertex{Vector3D(-1.0f, +1.0f, -1.0f), RColor::Black},
    //    RVertex{Vector3D(+1.0f, +1.0f, -1.0f), RColor::Blue},
    //    RVertex{Vector3D(+1.0f, -1.0f, -1.0f), RColor::Red},
    //    RVertex{Vector3D(-1.0f, -1.0f, +1.0f), RColor::Green},
    //    RVertex{Vector3D(-1.0f, +1.0f, +1.0f), RColor::Gray},
    //    RVertex{Vector3D(+1.0f, +1.0f, +1.0f), RColor::Yellow},
    //    RVertex{Vector3D(+1.0f, -1.0f, +1.0f), RColor::Green}
    //};
    //Obj1->mIndicesData = 
    //{
    //    0, 1, 2,
    //    0, 2, 3,

    //    4, 5, 6, 
    //    4, 6, 7,

    //    4, 5, 1, 
    //    4, 1, 0,
    //    
    //    2, 3, 6, 
    //    3, 6, 7
    //};

    // Objects.push_back(Obj1);
    //std::wstring RaiDenPath = L"E:/Workspace/RealEngine/build/bin/Debug/resources/GenShin/Beelzebul.pmx";
    std::string RaiDenPath = "E:/MyProject/DirectX-Graphics-Samples/MiniEngine/TestProject/HuangQuan/星穹铁道—黄泉（轴修复）.pmx"; 
    //std::string RaiDenPath = "E:/Workspace/DirectX-Graphics-Samples/MiniEngine/TestProject/HuangQuan/星穹铁道—黄泉（轴修复）.pmx"; 
    std::shared_ptr<RModelData> ModelData = RAssetImporter::ImportModelNew(RaiDenPath);
    
    std::shared_ptr<RPrimitiveObject> Obj(new RPrimitiveObject());
    Obj->SetModelData(ModelData, "Model1");
    //RaiDenShougunObj->IntializeBuffer();
    Objects.push_back(Obj);
}

void REngine::OnUpdate()
{
    RApplication::Get().ProcessInput();

    // update scene 
    GameViewportClient->Update();

    // Render 
    RViewInfo ViewInfo;
    ViewInfo.SetRenderWindow(RApplication::Get().GetMainWindow());
    ViewInfo.SetPrimitives(Objects);
    GameViewportClient->SetUpView(ViewInfo);
    
    // ViewInfo contain the primitive information to rendering .
    RRenderer::Get().DoRender(ViewInfo);
}

void REngine::Destoryed()
{
    RRenderer::Get().Destroy();
    RLOG(Info, "{}: Engine destory", __FUNCTION__); 
}