#include<iostream>
#include<Windows.h>
#include<tchar.h>
#include"window.h"
#include"rendering/rendSettings.h"
#include"ui/input.h"
using namespace std;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
            GetStates()[wParam] = true;
            break;
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
            wParam = (uMsg == WM_LBUTTONUP) ? 1 : ((uMsg == WM_RBUTTONUP) ? 2 : 16);
        case WM_KEYUP:
            GetStates()[wParam] = false;
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window::Window() : hinstance(GetModuleHandle(nullptr)) {
    LPCSTR CLASS_NAME = _T("Untitled Gam Class");

    HICON hIcon = (HICON) LoadImage(NULL, "res/iconfile.ico", IMAGE_ICON, 512, 512, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);

    WNDCLASS wndClass = {};
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hInstance = hinstance;
    wndClass.hIcon = hIcon;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpfnWndProc = WindowProc;

    RegisterClass(&wndClass);

    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    int width = GAM_WIDTH_ACTUAL;
    int height = GAM_HEIGHT_ACTUAL;

    RECT rect;
    rect.left = GAM_START_X;
    rect.top = GAM_START_Y;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    hwin = CreateWindowEx(0, CLASS_NAME, _T("Untitled Gam"), style, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, hinstance, NULL);

    ShowWindow(hwin, SW_SHOW);
}
Window::~Window() {
    LPCSTR CLASS_NAME = _T("Untitled Gam Class");
    UnregisterClass(CLASS_NAME, hinstance);
}
bool Window::ProcessMessages() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
        switch (msg.message) {
            case WM_QUIT:
                return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}
