#pragma once 
#include "Core.h"
#include <memory>
#include <vector>

class RSceneInfo;
class RGenericWindow;
class RPrimitiveObject;

struct RViewInfo
{
public:
    void AddPrimitive(const std::shared_ptr<RPrimitiveObject>& Primitive);
    void SetRenderWindow(const std::shared_ptr<RGenericWindow>& Window);
    void SetPrimitives(const std::vector<std::shared_ptr<RPrimitiveObject>>& InPrimitives);
    void SetViewProjectionMatrix(const Matrix4& InMat) { ViewProjectionMat = InMat; }
    Matrix4 GetViewProjectionMatrix() const { return ViewProjectionMat; } 
    std::vector<std::shared_ptr<RPrimitiveObject>> GetPrimitives() const ;
private:
    Matrix4 ViewMat;
    Matrix4 ProjectionMat;
    Matrix4 ViewProjectionMat;
    
    RSceneInfo* SceneInfor;
    
    std::shared_ptr<RGenericWindow> RenderWindow;
    std::vector<std::shared_ptr<RPrimitiveObject>> Primitives;
};