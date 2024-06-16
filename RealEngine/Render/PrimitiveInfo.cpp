#include "PrimitiveInfo.h"

#include "GraphicInterface.h"

void RPrimitiveObject::IntializeBuffer()
{
    const size_t VertexDataStrideSize = sizeof(RVertex);
    //mVertexBuffer = GGraphicInterface->CreateBuffer(mVertexData.data(),mVertexData.size()*VertexDataStrideSize,VertexDataStrideSize,mName);

    //mIndexBuffer = GGraphicInterface->CreateBuffer(mIndicesData.data(), mIndicesData.size() * sizeof(uint32_t), sizeof(uint32_t), mName);
}
