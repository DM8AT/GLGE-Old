#ifndef CML_NO_VEC_CASTS
#include "CMLVec2.h"
#include "CMLVec3.h"
#endif

#ifndef _CMLVEC4_H_
#define _CMLVEC4_H_

struct vec4
{
    float x,y,z,w;

    vec4();

    vec4(float xyzw);

    vec4(float x, float y, float z, float w);

#ifndef CML_NO_VEC_CASTS
    vec4(vec3 xyz, float w);

    vec4(float x, vec3 yzw);

    vec4(vec2 xy, float z, float w);

    vec4(float x, vec2 yz, float w);

    vec4(float x, float y, vec2 zw);

    vec4(vec2 xy, vec2 zw);
#endif

    vec4 operator+(vec4 v);

    void operator+=(vec4 v);

    vec4 operator-(vec4 v);

    void operator-=(vec4 v);

    float operator*(vec4 v);

    void operator*=(vec4 v);

    vec4 operator/(vec4 v);

    void operator/=(vec4 v);

    bool operator==(vec4 v);

    bool operator>=(vec4 v);

    bool operator<=(vec4 v);

    bool operator<(vec4 v);

    bool operator>(vec4 v);

    float length();

    void normalize();

    vec4 scale(vec4 v);
};

#endif