#include<iostream>
#include<stdint.h>
#include"../vec.h"
#include"lighting.h"
using namespace std;

vector<Light> lightList(1);

Light::Light(LightType t, uint32_t c, double dd, double f, double af) {
    type = t;
    color = c;
    deaddistance = dd;
    falloff = f;
    angularfalloff = af;
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    dir.x = 0;
    dir.y = -1;
    dir.z = 0;
    addToLightList(*this);
}
Light::Light(vec3 p, vec3 d, LightType t, uint32_t c, double dd, double f, double af) {
    pos = p;
    dir = d;
    type = t;
    color = c;
    deaddistance = dd;
    falloff = f;
    angularfalloff = af;
    addToLightList(*this);
}

vector<Light> getLightList() {
    return lightList;
}
vector<Light>& getActualLightList() {
    return lightList;
}
void addToLightList(Light& light) {
    lightList.push_back(light);
}
