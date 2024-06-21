#pragma once
#include "Core.h"

#include <vector>

struct RMeshData
{
	uint32_t vbOffset;
	uint32_t vbSIze;
	uint32_t vbStride;
	uint32_t ibOffset;
	uint32_t ibSize;
	Matrix4 xform;// all Local space  
	RQuat rotation; 
	Vector3D scale;
};

class RModelData
{
public:
private:
	std::vector<uint8_t> m_GeometryData;
	std::vector<RMeshData> m_MeshesData;
};