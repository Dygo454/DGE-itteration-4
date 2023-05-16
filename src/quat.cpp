#include<iostream>
#include<math.h>
#include"quat.h"
using namespace std;

#define M_PI 3.14159265358979323846

Quat::Quat(double oW, double oX, double oY, double oZ) {
    w = oW;
    x = oX;
    y = oY;
    z = oZ;
}
Quat Quat::eulerAngles(double ex, double ey, double ez) {
    double d2r = M_PI / 360.0;
    Quat result(cos(d2r*ex),sin(d2r*ex),0,0);
    result *= Quat(cos(d2r*ez),0,0,sin(d2r*ez));
    result *= Quat(cos(d2r*ey),0,sin(d2r*ey),0);
    result.normalize();
    return result;
}
Quat Quat::operator*(const Quat& a) {
    Quat newQ;
    newQ.w = (w*a.w - x*a.x - y*a.y - z*a.z);
    newQ.x = (x*a.w + w*a.x + y*a.z - z*a.y);
    newQ.y = (w*a.y - x*a.z + y*a.w + z*a.x);
    newQ.z = (w*a.z + x*a.y - y*a.x + z*a.w);
    return newQ;
}
Quat& Quat::operator*=(const Quat& a) {
    Quat result = (*this)*a;
    w = result.w;
    x = result.x;
    y = result.y;
    z = result.z;
    return *this;
}
void Quat::conjugate() {
    x *= -1;
    y *= -1;
    z *= -1;
}
void Quat::normalize() {
    double mag = sqrt((w*w)+(x*x)+(y*y)+(z*z));
    w /= mag;
    x /= mag;
    y /= mag;
    z /= mag;
}
