#include "Helper.h"

#include <fstream>
ComPtr<ID3D12Resource> D3DUtil::CreateDefaultBuffer(ID3D12Device* device, 
                                                    ID3D12GraphicsCommandList* cmdList, 
                                                    const void* initData, UINT64 byteSize, 
                                                    ComPtr<ID3D12Resource>& uploadBuffer)
{
    // actully buffer -- > Default heap 
    ComPtr<ID3D12Resource> defaultBuffer;
    CD3DX12_HEAP_PROPERTIES tempHeap(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC TempBufferRes = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
    ThrowIfFailed(device->CreateCommittedResource(&tempHeap, 
                                                  D3D12_HEAP_FLAG_NONE, 
                                                  &TempBufferRes, 
                                                  D3D12_RESOURCE_STATE_COPY_DEST,
                                                  nullptr, 
                                                  IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

    // Mid upload heap resource buffer --> upload heap
    CD3DX12_HEAP_PROPERTIES ttHeap(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC ttBufferRes = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
    ThrowIfFailed(device->CreateCommittedResource(&ttHeap, 
                                                  D3D12_HEAP_FLAG_NONE, 
                                                  &ttBufferRes, 
                                                  D3D12_RESOURCE_STATE_GENERIC_READ,
                                                  nullptr, 
                                                  IID_PPV_ARGS(uploadBuffer.GetAddressOf())));

    // descript the data to upload to default heap
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = initData; // data memory address 
    subResourceData.RowPitch = byteSize; // size
    subResourceData.SlicePitch = subResourceData.RowPitch;

    // CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
    // cmdList->ResourceBarrier(1, &barrier);

    UpdateSubresources<1>(cmdList,defaultBuffer.Get(), uploadBuffer.Get(), 0,0,1,&subResourceData);

    CD3DX12_RESOURCE_BARRIER barrier2 = 
                                    CD3DX12_RESOURCE_BARRIER::Transition(
                                        defaultBuffer.Get(), 
                                        D3D12_RESOURCE_STATE_COPY_DEST, 
                                        D3D12_RESOURCE_STATE_INDEX_BUFFER);

    cmdList->ResourceBarrier(1, &barrier2);

    return defaultBuffer;
}

UINT D3DUtil::CalcConstantBufferByteSize(UINT byteSize)
{
    return (byteSize + 255) & ~255;
}

ComPtr<ID3DBlob> D3DUtil::LoadBinary(const std::wstring& filename)
{
    std::ifstream fin(filename, std::ios::binary);
    fin.seekg(0, std::ios_base::end);
    std::ifstream::pos_type size = (int)fin.tellg();
    fin.seekg(0, std::ios_base::beg);

    ComPtr<ID3DBlob> blob;
    ThrowIfFailed(D3DCreateBlob(size, blob.GetAddressOf()));
    fin.read((char*)blob->GetBufferPointer(), size);
    fin.close();

    return blob;
}
