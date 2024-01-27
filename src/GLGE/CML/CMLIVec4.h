#ifndef CML_NO_VEC_CASTS
#include "CMLIVec2.h"
#include "CMLIVec3.h"
#endif

#include "CMLVec4.h"

#ifndef _CMLivec4_H_
#define _CMLivec4_H_

/**
 * @brief like vec4 but with integers
 */
class ivec4
{
public:
    int x,y,z,w;

    /**
     * @brief default constructor
     * 
     */
    ivec4();

    /**
     * @brief Construct a new ivec4
     * 
     * @param xyzw the value for all 4 axis
     */
    ivec4(int xyzw);

    /**
     * @brief Construct a new ivec4
     * 
     * @param x the value for the x axis
     * @param y the value for the y axis
     * @param z the value for the z axis
     * @param w the value for the z axis
     */
    ivec4(int x, int y, int z, int w);

#ifndef CML_NO_VEC_CASTS
    /**
     * @brief Construct a new ivec4
     * 
     * @param xyz the value for the x, y and z axis
     * @param w the value for the z axis
     */
    ivec4(ivec3 xyz, int w);

    /**
     * @brief Construct a new ivec4
     * 
     * @param x the value for the x axis
     * @param yzw the value for the y,z and w axis
     */
    ivec4(int x, ivec3 yzw);

    /**
     * @brief Construct a new ivec4
     * 
     * @param xy the value for the x and y axis
     * @param z the value for the z axis
     * @param w the value for the w axis
     */
    ivec4(ivec2 xy, int z, int w);

    /**
     * @brief Construct a new ivec4
     * 
     * @param x the value for the x axis
     * @param yz the value for the y and z axis
     * @param w the value for the w axis
     */
    ivec4(int x, ivec2 yz, int w);

    /**
     * @brief Construct a new ivec4
     * 
     * @param x the value for the x axis
     * @param y the value for the y axis
     * @param zw the value for the z and w axis
     */
    ivec4(int x, int y, ivec2 zw);

    /**
     * @brief Construct a new ivec4
     * 
     * @param xy the value for the x and y axis
     * @param zw the value for the z and w axis
     */
    ivec4(ivec2 xy, ivec2 zw);
#endif

    /**
     * @brief add two ivec4s
     * 
     * @param v the other ivec4 that should be added with this vector
     * @return ivec4 the sum of the vectors
     */
    ivec4 operator+(ivec4 v);

    /**
     * @brief add a ivec4 to this ivec4
     * 
     * @param v the ivec4 that should be added to this ivec4
     */
    void operator+=(ivec4 v);

    /**
     * @brief subtract two ivec4s
     * 
     * @param v the ivec4 to subtract from this ivec4
     * @return ivec4 the difference of this ivec4 and the other devec2
     */
    ivec4 operator-(ivec4 v);

    /**
     * @brief subtract an other ivec4 from this ivec4
     * 
     * @param v the other ivec4
     */
    void operator-=(ivec4 v);

    /**
     * @brief calculate the dot product of two ivec4s
     * 
     * @param v the other ivec4
     * @return int the dot product of this ivec4 and the other ivec4
     */
    int operator*(ivec4 v);

    /**
     * @brief scale this ivec4 with another ivec4
     * 
     * @param v the other vector to scale this vector with
     */
    void operator*=(ivec4 v);

    /**
     * @brief scale the vector with another ivec4
     * 
     * @param v the INVERSE of the other vector to scale with
     * @return ivec4 the scale of this ivec4 and the inverse of the other ivec4
     */
    ivec4 operator/(ivec4 v);

    /**
     * @brief scale this ivec4 with another ivec4
     * 
     * @param v the INVERSE of the other ivec4 to scale with
     */
    void operator/=(ivec4 v);

    /**
     * @brief check if two ivec4s are equal
     * 
     * @param v the ivec4 to compare with
     * @return true : both ivec4s are equal | 
     * @return false : the ivec4s are not equal
     */
    bool operator==(ivec4 v);

    /**
     * @brief check if an ivec4 is smaller or equal than another ivec4
     * 
     * @param v the ivec4 that should be equal or less than this ivec4
     * @return true : the ivec4 is less or equal than this ivec4 | 
     * @return false : the other ivec4 is larger than this ivec4
     */
    bool operator>=(ivec4 v);

    /**
     * @brief check if an ivec4 is larger or equal than another ivec4
     * 
     * @param v the ivec4 that should be equal or larger than this ivec4
     * @return true : the ivec4 is larger or equal than this ivec4 | 
     * @return false : the other ivec4 is smaler than this ivec4
     */
    bool operator<=(ivec4 v);

    /**
     * @brief check if an ivec4 is larger than another ivec4
     * 
     * @param v the ivec4 that should be larger than this ivec4
     * @return true : the ivec4 is larger than this ivec4 | 
     * @return false : the other ivec4 is equal or smaler than this ivec4
     */
    bool operator<(ivec4 v);

    /**
     * @brief check if an ivec4 is larger than another ivec4
     * 
     * @param v the ivec4 that should be equal than this ivec4
     * @return true : the ivec4 is larger than this ivec4 | 
     * @return false : the other ivec4 is equal or smaler than this ivec4
     */
    bool operator>(ivec4 v);

    /**
     * @brief calculate the length of this ivec4
     * 
     * @return float the length of the vector
     */
    float length();

    /**
     * @brief normalize this ivec4
     */
    void normalize();

    /**
     * @brief scale this ivec4 with another one
     * 
     * @param v the other ivec4
     * @return ivec4 this vector scaled with the other one
     */
    ivec4 scale(ivec4 v);
};

#endif