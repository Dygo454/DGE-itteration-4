#include<iostream>
#define M_PI 3.14159265358979323846
#include<math.h>
#include"rendering/rend.h"
#include"tempCube.h"
#include"vec.h"
#include"quat.h"
using namespace std;

Cube::Cube(vec3 pos, vec3 r, double sl, string textFile) {
    sideLength = sl;
    if (textFile == "") {
        tris[0] = new Triangle(vec3(sl/2,sl/2,sl/2),vec3(sl/2,-sl/2,sl/2),vec3(sl/2,-sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),NULL,&position,&rotation);//f
        tris[1] = new Triangle(vec3(sl/2,sl/2,sl/2),vec3(sl/2,-sl/2,-sl/2),vec3(sl/2,sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),NULL,&position,&rotation);
        tris[2] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(sl/2,sl/2,sl/2),vec3(sl/2,sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),NULL,&position,&rotation);//r
        tris[3] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(sl/2,sl/2,-sl/2),vec3(-sl/2,sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),NULL,&position,&rotation);
        tris[4] = new Triangle(vec3(sl/2,-sl/2,sl/2),vec3(-sl/2,-sl/2,sl/2),vec3(-sl/2,-sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),NULL,&position,&rotation);//l
        tris[5] = new Triangle(vec3(sl/2,-sl/2,sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(sl/2,-sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),NULL,&position,&rotation);
        tris[6] = new Triangle(vec3(-sl/2,-sl/2,sl/2),vec3(-sl/2,sl/2,sl/2),vec3(-sl/2,sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),NULL,&position,&rotation);//ba
        tris[7] = new Triangle(vec3(-sl/2,-sl/2,sl/2),vec3(-sl/2,sl/2,-sl/2),vec3(-sl/2,-sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),NULL,&position,&rotation);
        tris[8] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(-sl/2,-sl/2,sl/2),vec3(sl/2,-sl/2,sl/2),1,1,vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),NULL,&position,&rotation);//t
        tris[9] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(sl/2,-sl/2,sl/2),vec3(sl/2,sl/2,sl/2),1,1,vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),NULL,&position,&rotation);
        tris[10] = new Triangle(vec3(sl/2,sl/2,-sl/2),vec3(sl/2,-sl/2,-sl/2),vec3(-sl/2,-sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),NULL,&position,&rotation);//bo
        tris[11] = new Triangle(vec3(sl/2,sl/2,-sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(-sl/2,sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),NULL,&position,&rotation);
    }
    else {
        tris[0] = new Triangle(vec3(sl/2,sl/2,sl/2),vec3(sl/2,-sl/2,sl/2),vec3(sl/2,-sl/2,-sl/2),vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),textFile,&position,&rotation);//f
        tris[1] = new Triangle(vec3(sl/2,sl/2,sl/2),vec3(sl/2,-sl/2,-sl/2),vec3(sl/2,sl/2,-sl/2),vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),textFile,&position,&rotation);
        tris[2] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(sl/2,sl/2,sl/2),vec3(sl/2,sl/2,-sl/2),vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),textFile,&position,&rotation);//r
        tris[3] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(sl/2,sl/2,-sl/2),vec3(-sl/2,sl/2,-sl/2),vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),textFile,&position,&rotation);
        tris[4] = new Triangle(vec3(sl/2,-sl/2,sl/2),vec3(-sl/2,-sl/2,sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),textFile,&position,&rotation);//l
        tris[5] = new Triangle(vec3(sl/2,-sl/2,sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(sl/2,-sl/2,-sl/2),vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),textFile,&position,&rotation);
        tris[6] = new Triangle(vec3(-sl/2,-sl/2,sl/2),vec3(-sl/2,sl/2,sl/2),vec3(-sl/2,sl/2,-sl/2),vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),textFile,&position,&rotation);//ba
        tris[7] = new Triangle(vec3(-sl/2,-sl/2,sl/2),vec3(-sl/2,sl/2,-sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),textFile,&position,&rotation);
        tris[8] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(-sl/2,-sl/2,sl/2),vec3(sl/2,-sl/2,sl/2),vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),textFile,&position,&rotation);//t
        tris[9] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(sl/2,-sl/2,sl/2),vec3(sl/2,sl/2,sl/2),vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),textFile,&position,&rotation);
        tris[10] = new Triangle(vec3(sl/2,sl/2,-sl/2),vec3(sl/2,-sl/2,-sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),textFile,&position,&rotation);//bo
        tris[11] = new Triangle(vec3(sl/2,sl/2,-sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(-sl/2,sl/2,-sl/2),vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),textFile,&position,&rotation);
    }
    translate(pos);
    rot(Quat::eulerAngles(r.x,r.y,r.z));
}
Cube::Cube(vec3 pos, Quat r, double sl, string textFile) {
    sideLength = sl;
    if (textFile == "") {
        tris[0] = new Triangle(vec3(sl/2,sl/2,sl/2),vec3(sl/2,-sl/2,sl/2),vec3(sl/2,-sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),NULL,&position,&rotation);//f
        tris[1] = new Triangle(vec3(sl/2,sl/2,sl/2),vec3(sl/2,-sl/2,-sl/2),vec3(sl/2,sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),NULL,&position,&rotation);
        tris[2] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(sl/2,sl/2,sl/2),vec3(sl/2,sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),NULL,&position,&rotation);//r
        tris[3] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(sl/2,sl/2,-sl/2),vec3(-sl/2,sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),NULL,&position,&rotation);
        tris[4] = new Triangle(vec3(sl/2,-sl/2,sl/2),vec3(-sl/2,-sl/2,sl/2),vec3(-sl/2,-sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),NULL,&position,&rotation);//l
        tris[5] = new Triangle(vec3(sl/2,-sl/2,sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(sl/2,-sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),NULL,&position,&rotation);
        tris[6] = new Triangle(vec3(-sl/2,-sl/2,sl/2),vec3(-sl/2,sl/2,sl/2),vec3(-sl/2,sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),NULL,&position,&rotation);//ba
        tris[7] = new Triangle(vec3(-sl/2,-sl/2,sl/2),vec3(-sl/2,sl/2,-sl/2),vec3(-sl/2,-sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),NULL,&position,&rotation);
        tris[8] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(-sl/2,-sl/2,sl/2),vec3(sl/2,-sl/2,sl/2),1,1,vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),NULL,&position,&rotation);//t
        tris[9] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(sl/2,-sl/2,sl/2),vec3(sl/2,sl/2,sl/2),1,1,vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),NULL,&position,&rotation);
        tris[10] = new Triangle(vec3(sl/2,sl/2,-sl/2),vec3(sl/2,-sl/2,-sl/2),vec3(-sl/2,-sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),NULL,&position,&rotation);//bo
        tris[11] = new Triangle(vec3(sl/2,sl/2,-sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(-sl/2,sl/2,-sl/2),1,1,vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),NULL,&position,&rotation);
    }
    else {
        tris[0] = new Triangle(vec3(sl/2,sl/2,sl/2),vec3(sl/2,-sl/2,sl/2),vec3(sl/2,-sl/2,-sl/2),vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),textFile,&position,&rotation);//f
        tris[1] = new Triangle(vec3(sl/2,sl/2,sl/2),vec3(sl/2,-sl/2,-sl/2),vec3(sl/2,sl/2,-sl/2),vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),textFile,&position,&rotation);
        tris[2] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(sl/2,sl/2,sl/2),vec3(sl/2,sl/2,-sl/2),vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),textFile,&position,&rotation);//r
        tris[3] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(sl/2,sl/2,-sl/2),vec3(-sl/2,sl/2,-sl/2),vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),textFile,&position,&rotation);
        tris[4] = new Triangle(vec3(sl/2,-sl/2,sl/2),vec3(-sl/2,-sl/2,sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),textFile,&position,&rotation);//l
        tris[5] = new Triangle(vec3(sl/2,-sl/2,sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(sl/2,-sl/2,-sl/2),vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),textFile,&position,&rotation);
        tris[6] = new Triangle(vec3(-sl/2,-sl/2,sl/2),vec3(-sl/2,sl/2,sl/2),vec3(-sl/2,sl/2,-sl/2),vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),textFile,&position,&rotation);//ba
        tris[7] = new Triangle(vec3(-sl/2,-sl/2,sl/2),vec3(-sl/2,sl/2,-sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),textFile,&position,&rotation);
        tris[8] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(-sl/2,-sl/2,sl/2),vec3(sl/2,-sl/2,sl/2),vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),textFile,&position,&rotation);//t
        tris[9] = new Triangle(vec3(-sl/2,sl/2,sl/2),vec3(sl/2,-sl/2,sl/2),vec3(sl/2,sl/2,sl/2),vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),textFile,&position,&rotation);
        tris[10] = new Triangle(vec3(sl/2,sl/2,-sl/2),vec3(sl/2,-sl/2,-sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(1,0,0),vec3(0,0,0),vec3(0,1,0),textFile,&position,&rotation);//bo
        tris[11] = new Triangle(vec3(sl/2,sl/2,-sl/2),vec3(-sl/2,-sl/2,-sl/2),vec3(-sl/2,sl/2,-sl/2),vec3(1,0,0),vec3(0,1,0),vec3(1,1,0),textFile,&position,&rotation);
    }
    translate(pos);
    rot(r);
}
Cube::~Cube() {
    delete tris[0];
    delete tris[1];
    delete tris[2];
    delete tris[3];
    delete tris[4];
    delete tris[5];
    delete tris[6];
    delete tris[7];
    delete tris[8];
    delete tris[9];
    delete tris[10];
    delete tris[11];
}
void Cube::render(uint32_t color) {
    (*tris[0]).render(color);
    (*tris[1]).render(color);
    (*tris[2]).render(color);
    (*tris[3]).render(color);
    (*tris[4]).render(color);
    (*tris[5]).render(color);
    (*tris[6]).render(color);
    (*tris[7]).render(color);
    (*tris[8]).render(color);
    (*tris[9]).render(color);
    (*tris[10]).render(color);
    (*tris[11]).render(color);
}
void Cube::translate(vec3 v) {
    position += v;
}
void Cube::rot(double x, double y, double z) {
    rot(Quat::eulerAngles(x,y,z));
}
void Cube::rot(Quat r) {
    rotation *= r;
}
void Cube::setRot(double x, double y, double z) {
    setRot(Quat::eulerAngles(x,y,z));
}
void Cube::setRot(Quat r) {
    rotation = r;
}
