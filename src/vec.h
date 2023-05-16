#ifndef VEC_H
#define VEC_H

#include<stdint.h>
#include<CL/cl.hpp>
#include<stdint.h>
#include"quat.h"

struct vec3CL {
    double x;
    double y;
    double z;
};
struct vec3CLI {
    int x;
    int y;
    int z;
};

class vec3 {
public:
    vec3(double oX = 0, double oY = 0, double oZ = 0);
    vec3(const vec3& other);
    vec3 operator+(vec3 o);
    vec3 operator+=(vec3 o);
    vec3 operator-(vec3 o);
    vec3 operator-=(vec3 o);
    vec3 operator*(double a);
    vec3 operator*=(double a);
    vec3 operator/(double a);
    vec3 operator/=(double a);
    vec3CL toStruct();
    vec3CLI toStructI();
    double magnitude();
    void setMagnitude(double a);
    double scalarProj(vec3 o);
    vec3 vecProj(vec3 o);
    double dot(vec3 o);
    vec3 cross(vec3 o);
    void rot(Quat r);
    void eulerRot(double x, double y, double z);
    double x = 0;
    double y = 0;
    double z = 0;
    vec3 toScreenSpace();//TODO: make a to camera space func
};

class Triangle {
private:
    vec3 point1;
    vec3 point2;
    vec3 point3;
    vec3 uvPoint1;
    vec3 uvPoint2;
    vec3 uvPoint3;
    vec3* pivot;
    vec3 uvSize = vec3(0,0,0);
    uint32_t color = 0x00ffffff;
    cl::Buffer* texture = NULL;
    cl::Buffer* lightText = NULL;
    Quat* rotation;// do same as pivot where its a reference to a parent :p
    Quat lRotation;// do same as pivot where its a reference to a parent :p

    struct splitTri {
        Triangle* t1 = NULL;
        Triangle* t2 = NULL;
        int size = 0;
        ~splitTri() {
            if (t1 != NULL) {
                delete t1;
            }
            if (t2 != NULL) {
                delete t2;
            }
        }
    };

public:
    Triangle(vec3 p1=vec3(0,0,0), vec3 p2=vec3(0,0,0), vec3 p3=vec3(0,0,0), int sizeX=0, int sizeY=0, vec3 uv1=vec3(0,0,0), vec3 uv2=vec3(0,0,0), vec3 uv3=vec3(0,0,0), uint32_t* text=NULL, vec3* pi=NULL, Quat* ri=NULL);
    Triangle(vec3 p1, vec3 p2, vec3 p3, vec3 uv1, vec3 uv2, vec3 uv3, std::string file, vec3* pi=NULL, Quat* ri=NULL);
    ~Triangle();
    void flip();
    vec3& getPoint(int num=1);
    vec3 getPointRealSpace(int num=1);//TODO: make a get camera space func
    vec3& getUVPoint(int num=1);
    vec3 getUVSize() {return uvSize;}
    void render(uint32_t color=0x00ffffff);
    cl::Buffer* getTexture() {return texture;}
    cl::Buffer* getLightText() {return lightText;}
    void translate(vec3 v);
    void rot(Quat r);
    void rot(double x, double y, double z);
    void setRot(double x, double y, double z);
    void setRot(Quat r);
    void resetRot();
    splitTri split(vec3 pt, vec3 norm);
};

uint32_t* loadTextureTGA(std::string file, vec3* size=NULL);

#endif
