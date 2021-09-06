#pragma once 
#include "stdafx.h"
#include <stdexcept>

using Microsoft::WRL::ComPtr;

class D3DApp
{
public:
    void Setup() ;
    void LogAdapters() const ;
private:
    Microsoft::WRL::ComPtr<IDXGIFactory4> m_factory;
};

