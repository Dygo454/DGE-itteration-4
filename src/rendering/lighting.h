#ifndef LIGHTING_H
#define LIGHTING_H

#include<stdint.h>
#include"../vec.h"

enum LightType {
    Ambient,
    Directional,
    Point,
    Spot
};

class Light {
public:
    Light(LightType t=Directional, uint32_t c=0x00ffffff, double dd=0, double f=0, double af=0);
    Light(vec3 p, vec3 d, LightType t=Directional, uint32_t c=0xffffff, double dd=0, double f=0, double af=0);
    vec3 pos;
    vec3 dir;
    LightType type;
    uint32_t color;
    double deaddistance;
    double falloff;
    double angularfalloff;
};

std::vector<Light> getLightList();
std::vector<Light>& getActualLightList();
void addToLightList(Light& light);

#endif
