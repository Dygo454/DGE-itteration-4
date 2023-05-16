#include<iostream>
#include<time.h>
#include"time.h"
using namespace std;

double deltT = 0;
double fps = 0;

double getDeltaTime() {
    return deltT;
}
void setDeltaTime(double dt) {
    if (dt == 0) {
        dt = 1.0/CLOCKS_PER_SEC;
    }
    deltT = dt;
    fps = 1.0/dt;
}
double getFPS() {
    return fps;
}
