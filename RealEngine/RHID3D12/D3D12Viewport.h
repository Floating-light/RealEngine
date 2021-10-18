#pragma once 

#include "GraphicViewport.h"
#include "Windows/WindowsHeader.h"

class D3D12Viewport : public IGraphicViewport
{
public:
    D3D12Viewport(HWND handle, int width, int height);
    int m_width;
    int m_height;
    HWND m_windowHandle;
};