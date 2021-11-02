#pragma once
#include "stdafx.h"
#include <stdexcept>
#include <iostream>
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
};