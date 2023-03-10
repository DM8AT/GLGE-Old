#ifndef _CMLDVEC2_H_
#define _CMLDVEC2_H_
#pragma once

struct dvec2
{
    double x,y;

    dvec2();

    dvec2(double x, double y);

    dvec2 operator+(dvec2 v);

    void operator+=(dvec2 v);

    dvec2 operator-(dvec2 v);

    void operator-=(dvec2 v);

    double operator*(dvec2 v);

    void operator*=(dvec2 v);

    dvec2 operator/(dvec2 v);

    void operator/=(dvec2 v);

    bool operator==(dvec2 v);

    bool operator>=(dvec2 v);

    bool operator<=(dvec2 v);

    bool operator<(dvec2 v);

    bool operator>(dvec2 v);

    double length();

    void normalize();

    dvec2 scale(dvec2 v);
};

#endif