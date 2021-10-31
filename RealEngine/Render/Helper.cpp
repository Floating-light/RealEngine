#include "Helper.h"


ComPtr<ID3D12Resource> D3DUtil::CreateDefaultBuffer(ID3D12Device* device, 
                                                    ID3D12GraphicsCommandList* cmdList, 
                                                    const void* initData, UINT64 byteSize, 
                                                    ComPtr<ID3D12Resource>& uploadBuffer)
{
    // actully buffer -- > Default heap 
    ComPtr<ID3D12Resource> defaultBuffer;
    CD3DX12_HEAP_PROPERTIES tempHeap(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC TempBufferRes = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
    ThrowIfFailed(device->CreateCommittedResource(&tempHeap, D3D12_HEAP_FLAG_NONE, &TempBufferRes, D3D12_RESOURCE_STATE_COMMON,nullptr, IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

    // Mid upload heap resource buffer --> upload heap
    CD3DX12_HEAP_PROPERTIES ttHeap(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC ttBufferRes = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
    ThrowIfFailed(device->CreateCommittedResource(&ttHeap, D3D12_HEAP_FLAG_NONE, &ttBufferRes, D3D12_RESOURCE_STATE_COMMON,nullptr, IID_PPV_ARGS(uploadBuffer.GetAddressOf())));

    // descript the data to upload to default heap
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = initData;
    subResourceData.RowPitch = byteSize;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
    cmdList->ResourceBarrier(1, &barrier);

    UpdateSubresources<1>(cmdList,defaultBuffer.Get(), uploadBuffer.Get(), 0,0,1,&subResourceData);

    CD3DX12_RESOURCE_BARRIER barrier2 = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
    cmdList->ResourceBarrier(1, &barrier2);

    return defaultBuffer;
}
