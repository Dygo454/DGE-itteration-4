#include<iostream>
#include<string>
#define M_PI 3.14159265358979323846
#include<math.h>
#include<CL/cl.hpp>
#include<stdint.h>
#include<fstream>
#include"rendering/rend.h"
#include"quat.h"
#include"vec.h"
using namespace std;
using namespace cl;

vec3::vec3(double oX, double oY, double oZ) {
    x = oX;
    y = oY;
    z = oZ;
}
vec3::vec3(const vec3& other) {
    x = other.x;
    y = other.y;
    z = other.z;
}
vec3 vec3::operator+(vec3 o) {
    vec3 r;
    r.x = x + o.x;
    r.y = y + o.y;
    r.z = z + o.z;
    return r;
}
vec3 vec3::operator+=(vec3 o) {
    x += o.x;
    y += o.y;
    z += o.z;
    return *this;
}
vec3 vec3::operator-(vec3 o) {
    vec3 r;
    r.x = x - o.x;
    r.y = y - o.y;
    r.z = z - o.z;
    return r;
}
vec3 vec3::operator-=(vec3 o) {
    x -= o.x;
    y -= o.y;
    z -= o.z;
    return *this;
}
vec3 vec3::operator*(double a) {
    vec3 r;
    r.x = x * a;
    r.y = y * a;
    r.z = z * a;
    return r;
}
vec3 vec3::operator*=(double a) {
    x *= a;
    y *= a;
    z *= a;
    return *this;
}
vec3 vec3::operator/(double a) {
    vec3 r;
    r.x = x / a;
    r.y = y / a;
    r.z = z / a;
    return r;
}
vec3 vec3::operator/=(double a) {
    x /= a;
    y /= a;
    z /= a;
    return *this;
}
vec3CL vec3::toStruct() {
    vec3CL vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}
vec3CLI vec3::toStructI() {
    vec3CLI vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}
double vec3::magnitude() {
    return sqrt((x*x)+(y*y)+(z*z));
}
void vec3::setMagnitude(double a) {
    *this *= (a/magnitude());
}
double vec3::scalarProj(vec3 o) {
    return dot(o)/o.magnitude();
}
vec3 vec3::vecProj(vec3 o) {
    // cout << scalarProj(o) << "\n\n";
    o.setMagnitude(scalarProj(o));
    return o;
}
double vec3::dot(vec3 o) {
    return (x*o.x)+(y*o.y)+(z*o.z);
}
vec3 vec3::cross(vec3 o) {
    vec3 answer;
    answer.x = y*o.z - z*o.y;
    answer.y = z*o.x - x*o.z;
    answer.z = x*o.y - y*o.x;
    return answer;
}
void vec3::rot(Quat rot) {
    Quat pureQuat(0,x,y,z);
    Quat cRot(rot.w,-rot.x,-rot.y,-rot.z);
    Quat answer = rot*pureQuat*cRot;
    x = answer.x;
    y = answer.y;
    z = answer.z;
}
void vec3::eulerRot(double x, double y, double z) {
    rot(Quat::eulerAngles(x,y,z));
}
vec3 vec3::toScreenSpace() {
    double d2r = M_PI / 180.0;
    int fov = 90;
    double yScale = 1.0/tan(d2r*fov/2);
    double xScale = 1.0/tan(d2r*fov/2*GAM_WIDTH_ACTUAL/GAM_HEIGHT_ACTUAL);
    double n = 0.001;
    double f = 1000;
    double nearmfar = f-n;
    double m[] = {
        xScale, 0, 0, 0,
        0, yScale, 0, 0,
        0, 0, -f/nearmfar, -1,
        0, 0, -f*n/nearmfar, 0
    };

    vec3 result(0,0,0);
    double w = m[11]*z;
    result.x = (x*m[0] + y*m[4] + z*m[8] + 1*m[12])/w+1;
    result.x *= GAM_WIDTH/2;
    result.y = (x*m[1] + y*m[5] + z*m[9] + 1*m[13])/w-1;
    result.y *= GAM_HEIGHT/-2;
    result.z = (x*m[2] + y*m[6] + z*m[10] + 1*m[14])/w;
    return result;
}


Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3, int sizeX, int sizeY, vec3 uv1, vec3 uv2, vec3 uv3, uint32_t* text, vec3* pi, Quat* ri) {
    point1 = p1;
    point2 = p2;
    point3 = p3;
    uvPoint1 = uv1;
    uvPoint2 = uv2;
    uvPoint3 = uv3;
    pivot = pi;
    rotation = ri;
    uvSize = vec3(sizeX,sizeY,0);
    uint32_t lightT[sizeX*sizeY];
    for (int i = 0; i < sizeX*sizeY; i++) {
        lightT[i] = 0x00ffffff;
    }
    lightText = new Buffer(GetContext(), CL_MEM_READ_WRITE, sizeX*sizeY*4);
    GetQueue().enqueueWriteBuffer(*lightText, CL_TRUE, 0, sizeX*sizeY*4, lightT);
    if (text == NULL) {
        texture = NULL;
        return;
    }
    texture = new Buffer(GetContext(), CL_MEM_READ_WRITE, sizeX*sizeY*4);
    GetQueue().enqueueWriteBuffer(*texture, CL_TRUE, 0, sizeX*sizeY*4, text);
}
Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3, vec3 uv1, vec3 uv2, vec3 uv3, std::string file, vec3* pi, Quat* ri) {
    uint32_t* text = loadTextureTGA(file,&uvSize);
    point1 = p1;
    point2 = p2;
    point3 = p3;
    uvPoint1 = uv1;
    uvPoint2 = uv2;
    uvPoint3 = uv3;
    pivot = pi;
    rotation = ri;
    uint32_t lightT[(int)(uvSize.x*uvSize.y)];
    for (int i = 0; i < (int)(uvSize.x*uvSize.y); i++) {
        lightT[i] = 0x00ffffff;
    }
    lightText = new Buffer(GetContext(), CL_MEM_READ_WRITE, (int)(uvSize.x*uvSize.y));
    GetQueue().enqueueWriteBuffer(*lightText, CL_TRUE, 0, (int)(uvSize.x*uvSize.y), lightT);
    texture = new Buffer(GetContext(), CL_MEM_READ_WRITE, (int)(uvSize.x*uvSize.y));
    GetQueue().enqueueWriteBuffer(*texture, CL_TRUE, 0, (int)(uvSize.x*uvSize.y), text);
    delete[] text;
}
Triangle::~Triangle() {
    clReleaseMemObject((*texture)());
    clReleaseMemObject((*lightText)());
    delete texture;
    delete lightText;
}
void Triangle::flip() {
    vec3 temp = point1;
    point1 = point3;
    point3 = temp;
}
vec3& Triangle::getPoint(int num) {
    switch (num) {
        case 1:
            return point1;
        case 2:
            return point2;
        case 3:
            return point3;
        default:
            return point1;
    }
}
vec3 Triangle::getPointRealSpace(int num) {
    if (num == 1) {
        vec3 p1 = point1;
        if (pivot != NULL && rotation != NULL) {
            p1.rot(*rotation);
            p1 += *pivot;
        }
        return p1;
    }
    if (num == 2) {
        vec3 p2 = point2;
        if (pivot != NULL && rotation != NULL) {
            p2.rot(*rotation);
            p2 += *pivot;
        }
        return p2;
    }
    if (num == 3) {
        vec3 p3 = point3;
        if (pivot != NULL && rotation != NULL) {
            p3.rot(*rotation);
            p3 += *pivot;
        }
        return p3;
    }
    return getPointRealSpace(1);
}
vec3& Triangle::getUVPoint(int num) {
    switch (num) {
        case 1:
            return uvPoint1;
        case 2:
            return uvPoint2;
        case 3:
            return uvPoint3;
        default:
            return uvPoint1;
    }
}
void Triangle::render(uint32_t color) {
    if (texture == NULL) {
        vec3 p1 = point1;
        if (pivot != NULL && rotation != NULL) {
            p1.rot(*rotation);
            p1 += *pivot;
        }
        p1 += (GetCamPos()*-1);
        p1.eulerRot(GetCamRot().x*-1,GetCamRot().y*-1,GetCamRot().z*-1);
        vec3 p2 = point2;
        if (pivot != NULL && rotation != NULL) {
            p2.rot(*rotation);
            p2 += *pivot;
        }
        p2 += (GetCamPos()*-1);
        p2.eulerRot(GetCamRot().x*-1,GetCamRot().y*-1,GetCamRot().z*-1);
        vec3 p3 = point3;
        if (pivot != NULL && rotation != NULL) {
            p3.rot(*rotation);
            p3 += *pivot;
        }
        p3 += (GetCamPos()*-1);
        p3.eulerRot(GetCamRot().x*-1,GetCamRot().y*-1,GetCamRot().z*-1);
        ShadeWorldSpaceTri(p1,p2,p3,color);
        return;
    }
    TextureWorldSpaceTri(*this);
}
void Triangle::translate(vec3 v) {
    point1 += v;
    point2 += v;
    point3 += v;
}
void Triangle::rot(Quat r) {
    lRotation *= r;
    if (pivot == NULL) {
        point2 -= point1;
        point3 -= point1;
        point2.rot(r);
        point3.rot(r);
        point2 += point1;
        point3 += point1;
    }
    else {
        point1 -= *pivot;
        point2 -= *pivot;
        point3 -= *pivot;
        point1.rot(r);
        point2.rot(r);
        point3.rot(r);
        point1 += *pivot;
        point2 += *pivot;
        point3 += *pivot;
    }
}
void Triangle::setRot(Quat r) {
    resetRot();
    rot(r);
}
void Triangle::resetRot() {
    Quat tempR = lRotation;
    tempR.conjugate();
    rot(tempR);
}
Triangle::splitTri Triangle::split(vec3 pt, vec3 norm) {
    splitTri newTris;

    return newTris;
}

uint32_t* loadTextureTGA(string file, vec3* size) {
    ifstream textFile;
    int strLength = strlen(file.c_str());
    const char* fileC = file.c_str();
    if (fileC[strLength-1] != 'a' || fileC[strLength-2] != 'g' || fileC[strLength-3] != 't' || fileC[strLength-4] != '.') {
        cout << "Error: expected tga file! (file: \"" << file << "\")" << endl;
        exit(1);
    }
    textFile.open(file, ios::binary);
    if (!textFile.is_open()) {
        cout << "Error opening texture file! (file: \"" << file << "\")" << endl;
        exit(1);
    }
    char headerInfo[18];
    textFile.read(headerInfo,18);
    uint32_t width = (((int)headerInfo[13]) << 8) + ((int)headerInfo[12]);
    uint32_t height = (((int)headerInfo[15]) << 8) + ((int)headerInfo[14]);
    if (size != NULL) {
        size->x = (int)width;
        size->y = (int)height;
    }
    uint32_t* text = new uint32_t[width*height];
    for (int i = 0; i < width*height; i+=4) {
        char pixel[4];
        textFile.read(pixel,4);
        text[i] = ((~(((int)pixel[3]) << 24))&0xff000000) + (((int)pixel[2]) << 16) + (((int)pixel[1]) << 8) + (((int)pixel[0]));
    }
    textFile.close();
    return text;
}
