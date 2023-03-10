#ifndef _CMLDVEC4_H_
#define _CMLDVEC4_H_
#pragma once

struct dvec4
{
    double x,y,z,w;

    dvec4();

    dvec4(double x, double y, double z, double w);

    dvec4 operator+(dvec4 v);

    void operator+=(dvec4 v);

    dvec4 operator-(dvec4 v);

    void operator-=(dvec4 v);

    double operator*(dvec4 v);

    void operator*=(dvec4 v);

    dvec4 operator/(dvec4 v);

    void operator/=(dvec4 v);

    bool operator==(dvec4 v);

    bool operator>=(dvec4 v);

    bool operator<=(dvec4 v);

    bool operator<(dvec4 v);

    bool operator>(dvec4 v);

    double length();

    void normalize();

    dvec4 scale(dvec4 v);
};

#endif