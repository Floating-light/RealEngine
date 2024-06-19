#pragma once
#include <vector>
#include "DirectXMath.h"

struct MeshData
{
	uint32_t vbOffset;
	uint32_t vbSIze;
	uint32_t vbStride;
	uint32_t ibOffset;
	uint32_t ibSize;
	DirectX::XMMATRIX xform;// all Local space 
	Math::Quaternion rotation;
	Math::XMFLOAT3 scale;
	XM
};

class RModelData
{
public:
	std::vector<uint8_t> m_GeometryData;
	std::vector<MeshData> m_MeshesData;
private:
	
};