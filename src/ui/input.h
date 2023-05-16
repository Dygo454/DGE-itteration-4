#ifndef INPUT_H
#define INPUT_H

#include"../window.h"
#include"../vec.h"

bool GetKeyDown(int keyCode);
bool GetKey(int keyCode);
bool GetKeyUp(int keyCode);
bool* GetStates();
vec3 GetMouseIn();
void updateInput();
struct KeyCode {
    static const int LMB = VK_LBUTTON;
    static const int RMB = VK_RBUTTON;
    static const int MMB = VK_MBUTTON;
    static const int Backspace = VK_BACK;
    static const int Tab = VK_TAB;
    static const int Enter = VK_RETURN;
    static const int LShift = VK_LSHIFT;
    static const int RShift = VK_RSHIFT;
    static const int LCTRL = VK_LCONTROL;
    static const int RCTRL = VK_RCONTROL;
    static const int LAlt = VK_LMENU;
    static const int RAlt = VK_RMENU;
    static const int CapsLock = VK_CAPITAL;
    static const int Escape = VK_ESCAPE;
    static const int Space = VK_SPACE;
    static const int Left = VK_LEFT;
    static const int Up = VK_UP;
    static const int Right = VK_RIGHT;
    static const int Down = VK_DOWN;
    static const int NP0 = 0x30;
    static const int NP1 = 0x31;
    static const int NP2 = 0x32;
    static const int NP3 = 0x33;
    static const int NP4 = 0x34;
    static const int NP5 = 0x35;
    static const int NP6 = 0x36;
    static const int NP7 = 0x37;
    static const int NP8 = 0x38;
    static const int NP9 = 0x39;
    static const int F1 = VK_F1;
    static const int F2 = VK_F2;
    static const int F3 = VK_F3;
    static const int F4 = VK_F4;
    static const int F5 = VK_F5;
    static const int F6 = VK_F6;
    static const int F7 = VK_F7;
    static const int F8 = VK_F8;
    static const int F9 = VK_F9;
    static const int F10 = VK_F10;
    static const int F11 = VK_F11;
    static const int F12 = VK_F12;
    static const int F13 = VK_F13;
    static const int F14 = VK_F14;
    static const int F15 = VK_F15;
    static const int F16 = VK_F16;
    static const int F17 = VK_F17;
    static const int F18 = VK_F18;
    static const int F19 = VK_F19;
    static const int F20 = VK_F20;
    static const int F21 = VK_F21;
    static const int F22 = VK_F22;
    static const int F23 = VK_F23;
    static const int F24 = VK_F24;
    static const int Colon = VK_OEM_1;
    static const int Comma = VK_OEM_COMMA;
    static const int Period = VK_OEM_PERIOD;
    static const int Slash = VK_OEM_2;
    static const int A = 0x41;
    static const int B = 0x42;
    static const int C = 0x43;
    static const int D = 0x44;
    static const int E = 0x45;
    static const int F = 0x46;
    static const int G = 0x47;
    static const int H = 0x48;
    static const int I = 0x49;
    static const int J = 0x4A;
    static const int K = 0x4B;
    static const int L = 0x4C;
    static const int M = 0x4D;
    static const int N = 0x4E;
    static const int O = 0x4F;
    static const int P = 0x50;
    static const int Q = 0x51;
    static const int R = 0x52;
    static const int S = 0x53;
    static const int T = 0x54;
    static const int U = 0x55;
    static const int V = 0x56;
    static const int W = 0x57;
    static const int X = 0x58;
    static const int Y = 0x59;
    static const int Z = 0x5A;
};

#endif
