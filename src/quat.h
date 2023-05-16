#ifndef QUAT_H
#define QUAT_H

class Quat {
public:
    double x,y,z = 0;
    double w = 1;
    Quat(double oW = 1, double oX = 0, double oY = 0, double oZ = 0);
    static Quat eulerAngles(double ex, double ey, double ez);
    Quat operator*(const Quat& a);
    Quat& operator*=(const Quat& a);
    void conjugate();
    void normalize();
};

#endif
