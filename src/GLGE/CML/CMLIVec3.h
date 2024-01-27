//check if vec to vec casts should be possible
#ifndef CML_NO_VEC_CASTS
#include "CMLIVec2.h"
#endif

#ifndef _CMLivec3_H_
#define _CMLivec3_H_

/**
 * @brief like vec3 but with integers
 * 
 */
class ivec3
{
public:
    int x,y,z;

    /**
     * @brief default constructor
     */
    ivec3();

    /**
     * @brief Construct a new ivec3
     * 
     * @param xyz the value for all 3 axis
     */
    ivec3(int xyz);

    /**
     * @brief Construct a new ivec3
     * 
     * @param x the value for the x axis
     * @param y the value for the y axis
     * @param z the value for the z axis
     */
    ivec3(int x, int y, int z);

#ifndef CML_NO_VEC_CASTS
    /**
     * @brief Construct a new ivec3
     * 
     * @param xy the value for the x and y axis
     * @param z the value for the z axis
     */
    ivec3(ivec2 xy, int z);

    /**
     * @brief Construct a new ivec3
     * 
     * @param x the value for the x axis
     * @param yz the value for the y and z axis
     */
    ivec3(int x, ivec2 yz);
#endif

    /**
     * @brief add two ivec3s
     * 
     * @param v the other ivec3 that should be added with this vector
     * @return ivec3 the sum of the vectors
     */
    ivec3 operator+(ivec3 v);

    /**
     * @brief add a ivec3 to this ivec3
     * 
     * @param v the ivec3 that should be added to this ivec3
     */
    void operator+=(ivec3 v);

    /**
     * @brief subtract two ivec3s
     * 
     * @param v the ivec3 to subtract from this ivec3
     * @return ivec3 the difference of this ivec3 and the other devec2
     */
    ivec3 operator-(ivec3 v);

    /**
     * @brief subtract an other ivec3 from this ivec3
     * 
     * @param v the other ivec3
     */
    void operator-=(ivec3 v);

    /**
     * @brief calculate the dot product of two ivec3s
     * 
     * @param v the other ivec3
     * @return int the dot product of this ivec3 and the other ivec3
     */
    int operator*(ivec3 v);

    /**
     * @brief scale this ivec3 with another ivec3
     * 
     * @param v the other vector to scale this vector with
     */
    void operator*=(ivec3 v);

    /**
     * @brief scale the vector with another ivec3
     * 
     * @param v the INVERSE of the other vector to scale with
     * @return ivec3 the scale of this ivec3 and the inverse of the other ivec3
     */
    ivec3 operator/(ivec3 v);

    /**
     * @brief scale this ivec3 with another ivec3
     * 
     * @param v the INVERSE of the other ivec3 to scale with
     */
    void operator/=(ivec3 v);

    /**
     * @brief check if two ivec3s are equal
     * 
     * @param v the ivec3 to compare with
     * @return true : both ivec3s are equal | 
     * @return false : the ivec3s are not equal
     */
    bool operator==(ivec3 v);

    /**
     * @brief check if two ivec3s are not equal
     * 
     * @param v the ivec3 to compare with
     * @return true : both ivec3s are not equal | 
     * @return false : the ivec3s are equal
     */
    bool operator!=(ivec3 v);

    /**
     * @brief check if an ivec3 is smaller or equal than another ivec3
     * 
     * @param v the ivec3 that should be equal or less than this ivec3
     * @return true : the ivec3 is less or equal than this ivec3 | 
     * @return false : the other ivec3 is larger than this ivec3
     */
    bool operator>=(ivec3 v);

    /**
     * @brief check if an ivec3 is larger or equal than another ivec3
     * 
     * @param v the ivec3 that should be equal or larger than this ivec3
     * @return true : the ivec3 is larger or equal than this ivec3 | 
     * @return false : the other ivec3 is smaler than this ivec3
     */
    bool operator<=(ivec3 v);

    /**
     * @brief check if an ivec3 is larger than another ivec3
     * 
     * @param v the ivec3 that should be larger than this ivec3
     * @return true : the ivec3 is larger than this ivec3 | 
     * @return false : the other ivec3 is equal or smaler than this ivec3
     */
    bool operator<(ivec3 v);

    /**
     * @brief check if an ivec3 is larger than another ivec3
     * 
     * @param v the ivec3 that should be equal than this ivec3
     * @return true : the ivec3 is larger than this ivec3 | 
     * @return false : the other ivec3 is equal or smaler than this ivec3
     */
    bool operator>(ivec3 v);

    /**
     * @brief calculate the length of this ivec3
     * 
     * @return float the length of the vector
     */
    float length();

    /**
     * @brief normalize this ivec3
     */
    void normalize();

    /**
     * @brief scale this ivec3 with another one
     * 
     * @param v the other ivec3
     * @return ivec3 this vector scaled with the other one
     */
    ivec3 scale(ivec3 v);

    /**
     * @brief calculate the cross product of this vector and another vector
     * 
     * @param v the other vector for the cross product
     * @return vec3 the cross product
     */
    ivec3 cross(ivec3 v);
};

#endif