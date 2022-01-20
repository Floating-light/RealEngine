#pragma once 
#include <vector>

#include "Math/Vector.h"

struct RPrimitiveObject
{
public:
    std::vector<Vector3D> Vertex;
    std::vector<Vector3D> Color;
    std::vector<uint32_t> Indices;
};