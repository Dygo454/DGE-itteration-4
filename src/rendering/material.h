#ifndef MATERIAL_H
#define MATERIAL_H

#include<stdint.h>
#include"../vec.h"

class Material {
private:
    uint32_t color;
    vec3 reflectivity;
    double smoothness;
    double metallic;

public:
    Material(double s = 0.5,double m = 0, uint32_t c = 0xffffff);
    Material(vec3 r,double s = 0.5,double m = 0, uint32_t c = 0xffffff);
    uint32_t getColor() {return color;}//TODO: add or change to a way to texture mat
    void setColor(uint32_t c) {color = c;}//TODO: add or change to a way to texture mat
    vec3 getReflectivity() {return reflectivity;}
    void setReflectivity(vec3 r) {reflectivity = r;}
    double getSmoothness() {return smoothness;}
    void setSmoothness(double s) {smoothness = s;}
    double getMetallic() {return metallic;}
    void setMetallic(double m) {metallic = m;}
};

#endif
