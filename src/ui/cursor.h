#ifndef CURSOR_H
#define CURSOR_H

#include"../vec.h"

enum CursorState {
    CursorFree,
    CursorFreeHidden,
    CursorLooped,
    CursorLoopedHidden,
    CursorTrapped,
    CursorTrappedHidden,
    CursorLocked,
    CursorLockedHidden
};

void SetMousePos(int x, int y);
vec3 GetMousePos();
vec3 GetRelMousePos();
void SetCursorState(CursorState cs);
void TickCursorState();
void SetUpCursor(HWND hwnd);

#endif
