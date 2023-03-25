#ifndef _GLGE_MATH_H_
#define _GLGE_MATH_H_

#include "CML/CML.h"

mat4 glgeLookAt(vec3 eye, vec3 center, vec3 up);

vec3 glgeRotateVector(float angle, vec3 v, vec3 target);

#endif