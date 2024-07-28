#pragma once
#include "Core.h"

#include <vector>
#include "RHIBuffer.h"
#include "Texture.h"
#include "DescriptorHeap.h"

struct RMeshData
{
	uint32_t vbOffset;
	uint32_t vbSize;
	uint32_t vbStride;
	uint32_t ibOffset;
	uint32_t ibSize;
	uint32_t indexCount;
	uint32_t MaterialIndex;

	Matrix4 xform;// all Local space  
	RQuat rotation;
	Vector3D scale;

	std::string Name;
};

struct RMaterial
{
	RMaterial(const std::string& InPath)
		: DiffuseTexPath(InPath)
	{}

	std::string DiffuseTexPath;
	RTexture Texture{};
	RDescriptorHandle Handle{};
};
class RModelData
{
public:
	void SetName(const std::string& InName) { m_Name = InName; };
	void SetMaterials(const std::vector<RMaterial>& InMats) { m_Materials = InMats; };
	
	const RMaterial& GetMaterial(uint32_t InIndex)const;
	
	RDescriptorHandle GetMaterialSRV(int32_t InIndex)const;
	
	std::string GetMaterialName(int32_t InIndex) const;
	
	std::vector<uint8_t>& GetGeometryData() { return m_GeometryData; };
	void SetGeometryData(const std::vector<uint8_t>& InGeoData) { m_GeometryData = InGeoData; }

	void AddMeshData(const RMeshData& InData) { m_MeshesData.push_back(InData); };
	std::vector<RMeshData>& GetMeshesData() { return m_MeshesData; }
	const std::vector<RMeshData>& GetMeshesData()const { return m_MeshesData; }
	
	const RRHIBufferByteAddress& GetGeometryDataBuffer() const { return m_GeometryBuffer; }; 
	
	void PostLoad();
private:
	std::string m_Name;
	RRHIBufferByteAddress m_GeometryBuffer;
	std::vector<uint8_t> m_GeometryData;
	std::vector<RMeshData> m_MeshesData;
	std::vector<RMaterial> m_Materials{};
};

inline RDescriptorHandle RModelData::GetMaterialSRV(int32_t InIndex)const  
{ 
	RCHECK(InIndex < m_Materials.size());
	return m_Materials[InIndex].Handle; 
}

inline std::string RModelData::GetMaterialName(int32_t InIndex) const
{
	RCHECK(InIndex < m_Materials.size()); 
	return m_Materials[InIndex].DiffuseTexPath;  
}

inline const RMaterial& RModelData::GetMaterial(uint32_t InIndex)const
{
	RCHECK(InIndex < m_Materials.size());
	return m_Materials[InIndex];
}
