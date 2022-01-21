#pragma once 
#include <vector>

#include "Math/Vector.h"
#include "Math/Vector4D.h"

struct RVertex
{
    Vector3D Vertex;
    RColor Color;
};

struct RPrimitiveObject
{
public:
    std::vector<RVertex> VertexData;
    std::vector<uint32_t> Indices;
};