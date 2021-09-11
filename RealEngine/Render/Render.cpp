#include "stdafx.h"

#include <iostream>

#include "D3DApp.h"
#include "AppWindow.h"

                // "/Zi",
                // "/Fo:", "${workspaceFolder}\\debug\\", // output file path, .exe .obj
                // "/Fd:", "${workspaceFolder}\\debug\\", // .ilk .pdb 
                // "/EHsc",
                // "/D", "UNICODE",
                // "/Fe:", "${workspaceFolder}\\debug\\${fileBasenameNoExtension}.exe", // target output file
                // "${file}",
                // ".\\D3DApp.cpp"
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "d3dcompiler.lib")

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    D3DApp App(1280, 720, L"D3DApp");
    return AppWindow::Get().Run(&App, hInstance, nCmdShow);
}