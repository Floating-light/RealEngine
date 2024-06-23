#include "PrimitiveInfo.h"

#include "GraphicInterface.h"

void RPrimitiveObject::IntializeBuffer()
{
    //const size_t VertexDataStrideSize = sizeof(RVertex);
    //mVertexBuffer = GGraphicInterface->CreateBuffer(mVertexData.data(),mVertexData.size()*VertexDataStrideSize,VertexDataStrideSize,mName);

    //mIndexBuffer = GGraphicInterface->CreateBuffer(mIndicesData.data(), mIndicesData.size() * sizeof(uint32_t), sizeof(uint32_t), mName);
}

void RPrimitiveObject::SetModelData(std::shared_ptr<RModelData> InData, const std::string& Name)
{
    m_ModelData = InData;
    m_Name = Name; 
}
