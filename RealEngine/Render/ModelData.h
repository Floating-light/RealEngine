#pragma once
#include "Core.h"

#include <vector>
#include "RHIBuffer.h"

struct RMeshData
{
	uint32_t vbOffset;
	uint32_t vbSize;
	uint32_t vbStride;
	uint32_t ibOffset;
	uint32_t ibSize;
	uint32_t indexCount;

	Matrix4 xform;// all Local space  
	RQuat rotation; 
	Vector3D scale;
};

class RModelData
{
public:
	std::vector<uint8_t>& GetGeometryData() { return m_GeometryData; };
	std::vector<RMeshData>& GetMeshesData() { return m_MeshesData; }
	const std::vector<RMeshData>& GetMeshesData()const { return m_MeshesData; }
	const RRHIBufferByteAddress& GetGeometryDataBuffer() const { return m_GeometryBuffer; }; 
	void PostLoad();
private:
	RRHIBufferByteAddress m_GeometryBuffer;
	std::vector<uint8_t> m_GeometryData;
	std::vector<RMeshData> m_MeshesData;
};