#ifndef WINDOW_H
#define WINDOW_H

#include<Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
public:
    Window();
    Window(const Window&) = delete;
    Window* operator=(const Window&) = delete;
    ~Window();

    bool ProcessMessages();
    HWND& getWin() {return hwin;}
    HINSTANCE&  getInst() {return hinstance;}

private:
    HINSTANCE hinstance;
    HWND hwin;
};

#endif
