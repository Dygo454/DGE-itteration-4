#ifndef TEMPCUBE_H
#define TEMPCUBE_H

#include<stdint.h>
#include"vec.h"
#include"quat.h"

class Cube {
public:
    Cube(vec3 pos=vec3(0,0,0), vec3 r=vec3(0,0,0), double sl=1, std::string textFile="");
    Cube(vec3 pos, Quat q, double sl=1, std::string textFile="");
    ~Cube();
    void render(uint32_t Color = 0x00ffffff);
    void translate(vec3 v);
    void rot(Quat r);
    void rot(double x, double y, double z);
    void setRot(Quat r);
    void setRot(double x, double y, double z);
private:
    double sideLength = 1;
    vec3 position;
    Quat rotation;
    Triangle* tris[12];
};

#endif
