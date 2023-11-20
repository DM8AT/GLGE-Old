#ifndef CML_NO_VEC_CASTS
#include "CMLVec2.h"
#endif

#ifndef _CMLVEC3_H_
#define _CMLVEC3_H_

struct vec3
{
    float x,y,z;

    vec3();

    vec3(float xyz);

    vec3(float x, float y, float z);

#ifndef CML_NO_VEC_CASTS
    vec3(vec2 xy, float z);

    vec3(float x, vec2 yz);
#endif

    vec3 operator+(vec3 v);

    void operator+=(vec3 v);

    vec3 operator-(vec3 v);

    void operator-=(vec3 v);

    float operator*(vec3 v);

    vec3 operator*(float s);

    void operator*=(vec3 v);

    vec3 operator/(vec3 v);

    void operator/=(vec3 v);

    bool operator==(vec3 v);

    bool operator!=(vec3 v);

    bool operator>=(vec3 v);

    bool operator<=(vec3 v);

    bool operator<(vec3 v);

    bool operator>(vec3 v);

    float length();

    void normalize();

    vec3 cross(vec3 v);

    vec3 scale(vec3 v);
};

#endif