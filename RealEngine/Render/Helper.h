#pragma once
#include "stdafx.h"
#include <stdexcept>
#include <iostream>
#include <unordered_map>

#include "directxcollision.h"

using Microsoft::WRL::ComPtr;

inline std::string HrToString(HRESULT hr)
{
    char s_str[64];
    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
    return std::string(s_str);
}
class HrException : public std::runtime_error
{
public:
    HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) { }
    HRESULT Error() const { return m_hr;}
private:
    const HRESULT m_hr;
};

inline void ThrowIfFailed(HRESULT hr)
{
    if(FAILED(hr))
    {
        throw HrException(hr);
    }
}

// inline void OutputDebugString(const std::wstring& ws)
// {
//     std::wcout << ws << std::endl;
// }

class D3DUtil
{
public:
    // 对于不会改变的静态buffer, 通常上传到Default heap, 但CPU无法直接向其写入数据, 所以要用一个中介的Upload heap, 先把buffer写入到upload heap 的Resource 中(GPU), 再把其复制到Default heap中
    static ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* initData, UINT64 byteSize, ComPtr<ID3D12Resource>& uploadBuffer);
    static UINT CalcConstantBufferByteSize(UINT byteSize);
    // D3DUtil::LoadBinary(L"shaders\\color_vs.cso") load shader binary .
    static ComPtr<ID3DBlob> LoadBinary(const std::wstring& filename);
};

template <typename T>
class UploadBuffer
{
public:
    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer):m_isConstantBuffer(isConstantBuffer)
    {
        m_elementByteSize = sizeof(T);
        if(isConstantBuffer)
        {
            m_elementByteSize = D3DUtil::CalcConstantBufferByteSize(m_elementByteSize);
        }
        CD3DX12_HEAP_PROPERTIES heap(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(m_elementByteSize*elementCount);
        device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE,&desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_uploadBuffer));
        m_uploadBuffer->Map(0, nullptr,&m_mappedData );
        // wo do not need to unmap until we are done with the rsource 
        // however , we must not write to the resource while it is in 
        // use by the GPU(so we must use synchronization techniques)
    }
    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
    ~UploadBuffer()
    {
        if(m_uploadBuffer != nullptr)
        {
            m_uploadBuffer->Unmap(0, nullptr);
        }
        m_mappedData = nullptr;
    }
    ID3D12Resource* Resource() const 
    {
        return m_uploadBuffer.Get();
    }
    void CopyData(int elementIndex, const T& Data)
    {
        memcpy(&m_mappedData[elementIndex*m_elementByteSize], &Data, sizeof(T));
    }
private:
    bool m_isConstantBuffer;
    UINT m_elementByteSize;
    BYTE* m_mappedData;
    ComPtr<ID3D12Resource> m_uploadBuffer;
};

struct SubmeshGeometry
{
    UINT IndexCount = 0;
    UINT StartIndexLocation = 0;
    INT BaseVertexLocation = 0 ;
    DirectX::BoundingBox Bounds;
};

struct MeshGeometry
{
    std::wstring Name;
    ComPtr<ID3DBlob> VertexBufferCPU;
    ComPtr<ID3DBlob> IndexBufferCPU;

    ComPtr<ID3D12Resource> VertexBufferGPU;
    ComPtr<ID3D12Resource > IndexBufferGPU;

    ComPtr<ID3D12Resource> VertexBufferUploader;
    ComPtr<ID3D12Resource > IndexBufferUploader;

    UINT VertexByteStride;
    UINT VertexBufferByteSize;
    DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
    UINT IndexBufferByteSize;

    std::unordered_map<std::wstring, SubmeshGeometry> DrawArgs;

    D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const 
    {
        D3D12_VERTEX_BUFFER_VIEW vbv;
        vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
        vbv.StrideInBytes = VertexByteStride;
        vbv.SizeInBytes = VertexBufferByteSize;
        return vbv;
    }
    D3D12_INDEX_BUFFER_VIEW IndexBufferView() const 
    {
        D3D12_INDEX_BUFFER_VIEW ibv;
        ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
        ibv.Format = IndexFormat;
        ibv.SizeInBytes= IndexBufferByteSize;
        return ibv;
    }
};