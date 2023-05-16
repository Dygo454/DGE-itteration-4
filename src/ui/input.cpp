#include<iostream>
#include"../vec.h"
#include"input.h"
#include"cursor.h"
using namespace std;

bool states[254];
bool oldStates[254];
bool changed[254];
vec3 mouseIn(0,0,0);

bool GetKeyDown(int keyCode) {
    return changed[keyCode] && states[keyCode];
}
bool GetKey(int keyCode) {
    return states[keyCode];
}
bool GetKeyUp(int keyCode) {
    return changed[keyCode] && !states[keyCode];
}
bool* GetStates() {
    return states;
}
vec3 GetMouseIn() {
    return mouseIn;
}

void updateInput() {
    for (int i = 0; i < 254; i++) {
        changed[i] = states[i] ^ oldStates[i];
        oldStates[i] = states[i];
    }
    mouseIn = GetRelMousePos();
    TickCursorState();
}
