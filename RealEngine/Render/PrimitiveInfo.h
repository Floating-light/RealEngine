#pragma once 
#include <vector>

#include "Math/Vector.h"
#include "Math/Vector4D.h"
#include "GraphicInterface.h"
struct RVertex
{
    RVertex()
        :Vertex(Vector3D(0,0,0))
        ,Normal(Vector3D::XAxisVector)
        ,UV(),
        Color(0){}

    RVertex(const Vector3D& InVert,const RColor& InColor)
        :Vertex(InVert)
        ,Normal()
        ,UV()
        ,Color(InColor)
        {

        }

    Vector3D Vertex;
    Vector3D Normal;
    Vector2D UV;
    RColor Color;
};

struct RPrimitiveObject
{
public:
    std::vector<RVertex> mVertexData;
    std::vector<uint32_t> mIndicesData;
    std::string mName;

    void IntializeBuffer();
    TRefCountPtr<RRHIBuffer> mVertexBuffer;
    TRefCountPtr<RRHIBuffer> mIndexBuffer;
};