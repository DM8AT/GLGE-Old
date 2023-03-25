#include "GLGEMath.h"

mat4 glgeLookAt(vec3 eye, vec3 center, vec3 up)
{
    //create 3 vectors for the collums of the output matrix
    vec3 a,b,c;

    //get the difference from the eye to the center
    c = eye-center;
    //normalise that difference
    c.normalize();

    //store the up vector in b
    b = up;
    //a is a cross product from b and c
    a = b.cross(c);
    //recalculate the b vector
    b = c.cross(a);

    //normalise the length of a and b
    a.normalize();
    b.normalize();

    //create the output matrix
    mat4 out;

    //store the elements in the matrix
    out.m[0][0] = a.x;
    out.m[1][0] = a.y;
    out.m[2][0] = a.z;
    out.m[3][0] = -(a*eye);
    out.m[0][1] = b.x;
    out.m[1][1] = b.y;
    out.m[2][1] = b.z;
    out.m[3][1] = -(b*eye);
    out.m[0][2] = c.x;
    out.m[1][2] = c.y;
    out.m[2][2] = c.z;
    out.m[3][2] = -(c*eye);
    out.m[0][3] = 0;
    out.m[1][3] = 0;
    out.m[2][3] = 0;
    out.m[3][3] = 1.0f;

    //return the matrix
    return out;
}

vec3 glgeRotateVector(float angle, vec3 v, vec3 target)
{
    Quaternion RotationQ = Quaternion(angle, v);

    Quaternion ConjugateQ = RotationQ.conjugate();

    Quaternion W = (RotationQ * target) * ConjugateQ;

    return vec3(W.x,W.y,W.z);
}