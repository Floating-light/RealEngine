#include "Texture.h"
#include "GraphicInterface.h"
#include "CommandContext.h"
#include <DirectXTex.h>

void RTexture::Create2D(const std::string& InName, uint32_t RowPitchBytes, uint32_t InWidth, uint32_t InHeight, DXGI_FORMAT InFormat, const void* InData)
{
	Destroy();
	ID3D12Device* Device = GGraphicInterface->GetDeviceRaw();
	m_UsageState = D3D12_RESOURCE_STATE_COPY_DEST;

	m_Width = InWidth;
	m_Height = InHeight;
	m_Depth = 1;

	D3D12_RESOURCE_DESC desc{};
	desc.Width = InWidth;
	desc.Height = InHeight;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = InFormat;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_HEAP_PROPERTIES HeapProps{};
	HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	HeapProps.CreationNodeMask = 1;
	HeapProps.VisibleNodeMask = 1;

	ASSERTDX(Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &desc, 
		D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(m_Resource.ReleaseAndGetAddressOf())));
	m_Resource->SetName(RUtility::StringToWstring(InName).c_str());
	
	D3D12_SUBRESOURCE_DATA texResource{};
	texResource.pData = InData; // 原始数据
	texResource.RowPitch = RowPitchBytes; // 每行有多少个字节，width * bytes_per_pixel
	texResource.SlicePitch = RowPitchBytes * InHeight; // 当前Subresource(也称为Slice) 总共有多少个字节

	GGraphicInterface->InitializeTexture(*this, 1, &texResource); 

	if (m_CpuDescriptorHandle.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
	{
		m_CpuDescriptorHandle = GGraphicInterface->AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	Device->CreateShaderResourceView(m_Resource.Get(), nullptr, m_CpuDescriptorHandle); 
}

RTexture RTexture::CreateTexture(const std::filesystem::path InPath)
{
	RTexture RetVal{};

	DirectX::TexMetadata metaData{};
	DirectX::ScratchImage scratchImage{};
	std::string ext = InPath.extension().string();  
	
	if (InPath.extension() == ".tga")
	{
		ASSERTDX(DirectX::LoadFromTGAFile(InPath.c_str(),&metaData,scratchImage));
	}
	else
	{
		ASSERTDX(DirectX::LoadFromWICFile(InPath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData, scratchImage));
	}

	RetVal.Create2D("TestTex1", scratchImage.GetImages()->rowPitch, metaData.width, metaData.height, metaData.format, scratchImage.GetImages()->pixels);
	return RetVal;
}