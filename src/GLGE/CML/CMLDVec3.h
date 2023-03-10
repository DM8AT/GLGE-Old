#ifndef _CMLDVEC3_H_
#define _CMLDVEC3_H_
#pragma once

struct dvec3
{
    double x,y,z;

    dvec3();

    dvec3(double x, double y, double z);

    dvec3 operator+(dvec3 v);

    void operator+=(dvec3 v);

    dvec3 operator-(dvec3 v);

    void operator-=(dvec3 v);

    double operator*(dvec3 v);

    void operator*=(dvec3 v);

    dvec3 operator/(dvec3 v);

    void operator/=(dvec3 v);

    bool operator==(dvec3 v);

    bool operator!=(dvec3 v);

    bool operator>=(dvec3 v);

    bool operator<=(dvec3 v);

    bool operator<(dvec3 v);

    bool operator>(dvec3 v);

    double length();

    void normalize();

    dvec3 cross(dvec3 v);

    dvec3 scale(dvec3 v);
};

#endif