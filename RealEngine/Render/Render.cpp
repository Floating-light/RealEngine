#include "stdafx.h"

#include <iostream>

#include "D3DApp.h"
                // "/Zi",
                // "/Fo:", "${workspaceFolder}\\debug\\", // output file path, .exe .obj
                // "/Fd:", "${workspaceFolder}\\debug\\", // .ilk .pdb 
                // "/EHsc",
                // "/D", "UNICODE",
                // "/Fe:", "${workspaceFolder}\\debug\\${fileBasenameNoExtension}.exe", // target output file
                // "${file}",
                // ".\\D3DApp.cpp"
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3D12.lib")
_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    D3DApp App;
    App.Setup();
    // App.LogAdapters();
    std::cout << "------------------->> End" << std::endl;
    return -1;
}