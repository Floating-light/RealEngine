#include "PixelBuffer.h"

DXGI_FORMAT RPixelBuffer::GetBaseFormat(DXGI_FORMAT defaultFormat)
{
    // D开头表示 Depth， S开头表示Stencil，RGB + TYPELIESS 则可以解释为任何你喜欢的类型。
    // 想要用作DepthBuffer，必须用D开头的类型。DirectX中对它有特殊处理
    // TYPELESS 的可以解释为Depth的类型，所以这个也可以用作DepthBuffer
    
    // 重新映射到TYPELESS，以便可以创建不同的View。
    // UNORM 标准化的无符号整数，归一化到[0,1]
    switch (defaultFormat)
    {
    case DXGI_FORMAT_R8G8B8A8_UNORM://  每个组件都是标准到[0,1]的8位无符号整数
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        return DXGI_FORMAT_R8G8B8A8_TYPELESS;

    case DXGI_FORMAT_B8G8R8A8_UNORM:
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        return DXGI_FORMAT_B8G8R8A8_TYPELESS;

    case DXGI_FORMAT_B8G8R8X8_UNORM:
    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        return DXGI_FORMAT_B8G8R8X8_TYPELESS;

        // 32-bit Z w/ Stencil
    case DXGI_FORMAT_R32G8X24_TYPELESS: // 32bit , 8bit , 24bit unuse, 其中都是无类型的
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:// 32位部分是float，depth，8位部分是stencil，uint
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:// 32位部分是float，8位部分是无类型
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
        return DXGI_FORMAT_R32G8X24_TYPELESS;

        // No Stencil
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT:
    case DXGI_FORMAT_R32_FLOAT:
        return DXGI_FORMAT_R32_TYPELESS;

        // 24-bit Z
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
        return DXGI_FORMAT_R24G8_TYPELESS;

        // 16-bit Z w/o Stencil
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_D16_UNORM:
    case DXGI_FORMAT_R16_UNORM:
        return DXGI_FORMAT_R16_TYPELESS;

    default:
        return defaultFormat;
    }
}

DXGI_FORMAT RPixelBuffer::GetStencilFormat(DXGI_FORMAT InFormat)
{
    switch (InFormat)
    {
    // 32-bit Z w/ Stencil
    case DXGI_FORMAT_R32G8X24_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
        return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
    // 24-bit Z
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
        return DXGI_FORMAT_X24_TYPELESS_G8_UINT; 
    default:
        return DXGI_FORMAT_UNKNOWN;
    }
}

DXGI_FORMAT RPixelBuffer::GetDSVFormat(DXGI_FORMAT InFormat)
{
    switch (InFormat)
    {
        // 32-bit Z w/ Stencil
    case DXGI_FORMAT_R32G8X24_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

        // No Stencil
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT:
    case DXGI_FORMAT_R32_FLOAT:
        return DXGI_FORMAT_D32_FLOAT;

        // 24-bit Z
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;

        // 16-bit Z w/o Stencil
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_D16_UNORM:
    case DXGI_FORMAT_R16_UNORM:
        return DXGI_FORMAT_D16_UNORM;

    default:
        return InFormat;
    }
}

DXGI_FORMAT RPixelBuffer::GetDepthFormat(DXGI_FORMAT Format)
{
    switch (Format)
    {
        // 32-bit Z w/ Stencil
    case DXGI_FORMAT_R32G8X24_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
        return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;

        // No Stencil
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT:
    case DXGI_FORMAT_R32_FLOAT:
        return DXGI_FORMAT_R32_FLOAT;

        // 24-bit Z
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
        return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

        // 16-bit Z w/o Stencil
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_D16_UNORM:
    case DXGI_FORMAT_R16_UNORM:
        return DXGI_FORMAT_R16_UNORM;

    default:
        return DXGI_FORMAT_UNKNOWN;
    }
}

D3D12_RESOURCE_DESC RPixelBuffer::DescribeTexture2D(uint32_t InWidth, uint32_t InHeight, uint32_t DepthOrArraySize, uint32_t NumMips, DXGI_FORMAT Format, uint32_t Flags)
{
	m_Width = InWidth;
	m_Height = InHeight;
	m_ArraySize = DepthOrArraySize;
	m_Format = Format;

	D3D12_RESOURCE_DESC Desc = {};
	Desc.Alignment = 0;
	Desc.DepthOrArraySize = (uint16_t)DepthOrArraySize;
	Desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	Desc.Flags = (D3D12_RESOURCE_FLAGS)Flags;
    Desc.Format = GetBaseFormat(Format);
    Desc.Height = InHeight;
    Desc.Width = InWidth;
    Desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN; // 让驱动或硬件确定最佳布局
    Desc.MipLevels = NumMips;
    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

	return Desc;
}

void RPixelBuffer::AssociateWithResource(ID3D12Device* Device, const std::string& InName, ID3D12Resource* InResource, D3D12_RESOURCE_STATES CurrentState)
{
    RCHECK(InResource);
    D3D12_RESOURCE_DESC ResourceDesc = InResource->GetDesc();

    m_Resource.Attach(InResource);
    m_UsageState = CurrentState;

    m_Width = ResourceDesc.Width;
    m_Height = ResourceDesc.Height;
    m_ArraySize = ResourceDesc.DepthOrArraySize;
    m_Format = ResourceDesc.Format;

    m_Resource->SetName(RUtility::StringToWstring(InName).c_str());
}

void RPixelBuffer::CreateTextureResource(ID3D12Device* Device, const std::string& InName, const D3D12_RESOURCE_DESC& InResourceDesc,
    D3D12_CLEAR_VALUE ClearValue, D3D12_GPU_VIRTUAL_ADDRESS VitualAddress)
{
    Destroy();

    CD3DX12_HEAP_PROPERTIES HeapProps(D3D12_HEAP_TYPE_DEFAULT);

    // D3D12_RESOURCE_STATE_COMMON 表明资源处于通用的初始化状态，
    // 如果是UploadHeap的资源可以进行CPU访问，上传数据
    // DefaultHeap 则必须转为Copy Dest 状态才可向它复制数据
    // 但不能用于 RT，SRV，D/S，Present，这些都要求特定的状态

    ASSERTDX(Device->CreateCommittedResource(&HeapProps,D3D12_HEAP_FLAG_NONE,
        &InResourceDesc,D3D12_RESOURCE_STATE_COMMON,&ClearValue,IID_PPV_ARGS(&m_Resource)));
    m_UsageState = D3D12_RESOURCE_STATE_COMMON;

    m_GpuVirtualAddress = D3D12_GPU_VIRTUAL_ADDRESS_NULL;

    m_Resource->SetName(RUtility::StringToWstring(InName).c_str());
}
