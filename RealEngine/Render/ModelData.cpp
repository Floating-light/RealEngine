#include "ModelData.h"
#include "Render.h"
#include "GraphicInterface.h"

void RModelData::PostLoad()
{
	ID3D12Device* Device = GGraphicInterface->GetDeviceRaw();
	m_GeometryBuffer.Create("ModelData", m_GeometryData.size(), 1, m_GeometryData.data());
	for (size_t i = 0; i < m_Materials.size(); i++)
	{
		m_Materials[i].Texture = RTexture::CreateTexture(m_Materials[i].DiffuseTexPath); 
		m_Materials[i].Handle = RRenderer::Get().AllocateSRV();

		Device->CopyDescriptorsSimple(1, m_Materials[i].Handle,
			m_Materials[i].Texture.GetSRV(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
}
