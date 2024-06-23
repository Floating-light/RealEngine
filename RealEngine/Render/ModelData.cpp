#include "ModelData.h"

void RModelData::PostLoad()
{
	m_GeometryBuffer.Create("ModelData", m_GeometryData.size(), 1, m_GeometryData.data());
}
