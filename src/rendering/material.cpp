#include<iostream>
#include<stdint.h>
#include"../vec.h"
#include"material.h"
using namespace std;

Material::Material(double s,double m, uint32_t c) {
    reflectivity.x = 0;
    reflectivity.y = 0;
    reflectivity.z = 0;
    smoothness = s;
    metallic = m;
    color = c;
}
Material::Material(vec3 r,double s,double m, uint32_t c) {
    reflectivity = r;
    smoothness = s;
    metallic = m;
    color = c;
}
