#ifndef CML_NO_VEC_CASTS
#include "CMLDVec2.h"
#include "CMLDVec3.h"
#endif

#ifndef _CMLDVEC4_H_
#define _CMLDVEC4_H_

struct dvec4
{
    double x,y,z,w;

    dvec4();

    dvec4(double xyzw);

    dvec4(double x, double y, double z, double w);

#ifndef CML_NO_VEC_CASTS
    dvec4(dvec3 xyz, double w);

    dvec4(double x, dvec3 yzw);

    dvec4(dvec2 xy, double z, double w);

    dvec4(double x, dvec2 yz, double w);

    dvec4(double x, double y, dvec2 zw);

    dvec4(dvec2 xy, dvec2 zw);
#endif

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