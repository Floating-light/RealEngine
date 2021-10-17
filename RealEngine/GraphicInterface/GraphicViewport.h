#pragma once 
#include "Windows/WindowsHeader.h"

class IGraphicViewport
{
public:
    IGraphicViewport(HWND handle, int width, int height);
    int m_width;
    int m_height;
    HWND m_windowHandle;
};