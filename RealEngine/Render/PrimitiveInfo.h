#pragma once 
#include <vector>
#include <string>
#include <memory>

#include "Transform.h"
#include "RHIBuffer.h"

class RModelData;

__declspec(align(256)) struct RModelConstantData
{
    Matrix4 ModelMatrix;
};
struct RPrimitiveObject
{
public:
    void IntializeBuffer();
    void SetModelData(std::shared_ptr<RModelData> InData, const std::string& Name, const RTransform& Intransform);
    RModelData* GetModelData()const { return m_ModelData.get(); } 
    D3D12_GPU_VIRTUAL_ADDRESS GetModelConstant()const { return m_ModelConstant.GetGPUVirtualAddress(); }
    void SetLocation(const Vector3D InLocation) { m_Transform.SetTranslation(InLocation); }
private:
    // Transofrm
    RTransform m_Transform;
    RRHIBufferByteAddress m_ModelConstant;

    std::string m_Name;
    std::shared_ptr<RModelData> m_ModelData; 
};