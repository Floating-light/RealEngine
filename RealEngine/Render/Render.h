#pragma once 

#include "ViewInfo.h"
#include "DepthBuffer.h"
#include "Texture.h"

class RGraphicPSO;

class RRenderer
{
public:
    static RRenderer& Get();
    
    void Init(std::shared_ptr<RGenericWindow> Window);
    
    // Called from engine every frame to rendering scene
    void DoRender(RViewInfo& ViewInfo);

    void Destroy();
private:
    //Microsoft::WRL::ComPtr<ID3D12Resource> m_DepthBuffer;
    RDepthBuffer m_SceneDepthBuffer;
    RTexture m_DefaultTexture;

    std::shared_ptr<RGraphicPSO> m_DefaultPSO;
    uint64_t m_FrameAsyncFence;
};