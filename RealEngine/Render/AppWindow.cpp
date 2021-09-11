#include "AppWindow.h"

#include "D3DApp.h"

AppWindow& AppWindow::Get()
{
    static AppWindow Instance;
    return Instance;
}

int AppWindow::Run(class D3DApp* Graphics, HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEX windowClass = { 0};
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"DXEngineClass";
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, static_cast<LONG>(Graphics->GetWidth()), static_cast<LONG>(Graphics->GetHeight())};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    m_hwnd = CreateWindow(
        windowClass.lpszClassName,
        Graphics->GetTitle(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr, 
        nullptr, 
        hInstance,
        Graphics);

    Graphics->Setup();

    ShowWindow(m_hwnd, nCmdShow);

    // Main loop 
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if(PeekMessage(&msg, NULL, 0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Graphics->OnDestroy();

    return static_cast<char>(msg.wParam);
}

LRESULT CALLBACK AppWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM  lParam)
{
    D3DApp* pGraphics = reinterpret_cast<D3DApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    switch(message)
    {
    case WM_CREATE:
        {
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        }
        return 0;
    case WM_KEYDOWN:
        if(pGraphics)
        {
            pGraphics->OnKeyDown(static_cast<UINT8>(wParam));
        }
        return 0;
    case WM_KEYUP:
        if(pGraphics)
        {
            pGraphics->OnKeyUp(static_cast<UINT8>(wParam));
        }
        return 0;
    case WM_PAINT:
        if(pGraphics)
        {
            pGraphics->OnUpdate();
            pGraphics->OnRender();
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}