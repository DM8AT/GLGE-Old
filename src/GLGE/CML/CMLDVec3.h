//check if vec to vec casts should be possible
#ifndef CML_NO_VEC_CASTS
#include "CMLDVec2.h"
#endif

#ifndef _CMLDVEC3_H_
#define _CMLDVEC3_H_

struct dvec3
{
    double x,y,z;

    dvec3();

    dvec3(double xyz);

    dvec3(double x, double y, double z);

#ifndef CML_NO_VEC_CASTS
    dvec3(dvec2 xy, double z);

    dvec3(double x, dvec2 yz);
#endif

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