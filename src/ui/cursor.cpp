#include<iostream>
#include<Windows.h>
#include"../rendering/rendSettings.h"
#include"../vec.h"
#include"cursor.h"
using namespace std;

CursorState cursor = CursorFree;
HWND window;
vec3 lastCursorPos(0,0,0);
bool wasFocused = false;
bool first = true;

void SetMousePos(int x, int y) {
    if (window != GetFocus()) {
        return;
    }
    RECT r;
    GetWindowRect(window, &r);
    SetCursorPos(x+r.left,y+r.top);
    lastCursorPos.x = x;
    lastCursorPos.y = y;
}
vec3 GetMousePos() {
    if (!wasFocused || window != GetFocus()) {
        if (window == GetFocus()) {
            wasFocused = true;
        }
        vec3 ret(0,0,0);
        return ret;
    }
    POINT point;
    GetCursorPos(&point);
    vec3 p(point.x, point.y, 0);
    RECT r;
    GetWindowRect(window, &r);
    vec3 winPos(r.left,r.top,0);
    p -= winPos;
    return p;
}
vec3 GetRelMousePos() {
    if (!wasFocused || window != GetFocus() || first) {
        if (window == GetFocus()) {
            wasFocused = true;
        }
        first = false;
        vec3 ret(0,0,0);
        return ret;
    }
    vec3 p(lastCursorPos.x, lastCursorPos.y, 0);
    p -= GetMousePos();
    p *= -1;
    return p;
}
void SetCursorState(CursorState cs) {
    if (cs == CursorFree || cs == CursorTrapped || cs == CursorLooped || cs == CursorLocked) {
        ShowCursor(true);
    }
    else {
        ShowCursor(false);
    }
    cursor = cs;
    lastCursorPos = GetMousePos();
    if (cs == CursorLocked || cs == CursorLockedHidden) {
        lastCursorPos.x = GAM_WIDTH_ACTUAL/2;
        lastCursorPos.y = GAM_HEIGHT_ACTUAL/2;
    }
}
void TickCursorState() {
    if (window != GetFocus()) {
        wasFocused = false;
        return;
    }
    RECT r;
    GetWindowRect(window, &r);
    vec3 mouse;
    switch (cursor) {
        case CursorFree:
            break;
        case CursorFreeHidden:
            break;
        case CursorLooped:
            mouse = GetMousePos();
            if (mouse.x < 0) {
                SetMousePos(GAM_WIDTH_ACTUAL-1,mouse.y);
            }
            else if (mouse.x >= GAM_WIDTH_ACTUAL) {
                SetMousePos(0,mouse.y);
            }
            if (mouse.y < 0) {
                SetMousePos(mouse.x, (r.bottom-r.top)-1);
            }
            else if (mouse.y >= (r.bottom-r.top)) {
                SetMousePos(mouse.x, (r.bottom-r.top)-GAM_HEIGHT_ACTUAL);
            }
            break;
        case CursorLoopedHidden:
            mouse = GetMousePos();
            if (mouse.x < 0) {
                SetMousePos(GAM_WIDTH_ACTUAL-1,mouse.y);
            }
            else if (mouse.x >= GAM_WIDTH_ACTUAL) {
                SetMousePos(0,mouse.y);
            }
            if (mouse.y < 0) {
                SetMousePos(mouse.x, (r.bottom-r.top)-1);
            }
            else if (mouse.y >= (r.bottom-r.top)) {
                SetMousePos(mouse.x, (r.bottom-r.top)-GAM_HEIGHT_ACTUAL);
            }
            break;
        case CursorTrapped:
            mouse = GetMousePos();
            if (mouse.x < 0) {
                SetMousePos(0,mouse.y);
            }
            else if (mouse.x >= GAM_WIDTH_ACTUAL) {
                SetMousePos(GAM_WIDTH-1,mouse.y);
            }
            if (mouse.y < 0) {
                SetMousePos(mouse.x, 0);
            }
            else if (mouse.y >= (r.bottom-r.top)) {
                SetMousePos(mouse.x, (r.bottom-r.top)-1);
            }
            break;
        case CursorTrappedHidden:
            mouse = GetMousePos();
            if (mouse.x < 0) {
                SetMousePos(0,mouse.y);
            }
            else if (mouse.x >= GAM_WIDTH_ACTUAL) {
                SetMousePos(GAM_WIDTH_ACTUAL-1,mouse.y);
            }
            if (mouse.y < 0) {
                SetMousePos(mouse.x, 0);
            }
            else if (mouse.y >= (r.bottom-r.top)) {
                SetMousePos(mouse.x, (r.bottom-r.top)-1);
            }
            break;
        case CursorLocked:
            SetMousePos(lastCursorPos.x,lastCursorPos.y);
            return;
        case CursorLockedHidden:
            SetMousePos(lastCursorPos.x,lastCursorPos.y);
            return;
    }
    lastCursorPos = GetMousePos();
}
void SetUpCursor(HWND hwnd) {
    window = hwnd;
}
