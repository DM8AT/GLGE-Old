#ifndef CML_NO_VEC_CASTS
#include "CMLVec2.h"
#endif

#ifndef _CMLVEC3_H_
#define _CMLVEC3_H_

/**
 * @brief a simple 3D vector
 * 
 */
class vec3
{
public:
    float x,y,z;

    /**
     * @brief default constructor
     */
    vec3();

    /**
     * @brief Construct a new vec3
     * 
     * @param xyz the value for all 3 axis
     */
    vec3(float xyz);

    /**
     * @brief Construct a new vec3
     * 
     * @param x the value for the x axis
     * @param y the value for the y axis
     * @param z the value for the z axis
     */
    vec3(float x, float y, float z);

#ifndef CML_NO_VEC_CASTS
    /**
     * @brief Construct a new vec3
     * 
     * @param xy the value for the x and y axis
     * @param z the value for the z axis
     */
    vec3(vec2 xy, float z);

    /**
     * @brief Construct a new vec3
     * 
     * @param x the value for the x axis
     * @param yz the value for the y and z axis
     */
    vec3(float x, vec2 yz);
#endif

    /**
     * @brief add two vec3s
     * 
     * @param v the other vec3 that should be added with this vector
     * @return vec3 the sum of the vectors
     */
    vec3 operator+(vec3 v);

    /**
     * @brief add a vec3 to this vec3
     * 
     * @param v the vec3 that should be added to this vec3
     */
    void operator+=(vec3 v);

    /**
     * @brief subtract two vec3s
     * 
     * @param v the vec3 to subtract from this vec3
     * @return vec3 the difference of this vec3 and the other devec2
     */
    vec3 operator-(vec3 v);

    /**
     * @brief subtract an other vec3 from this vec3
     * 
     * @param v the other vec3
     */
    void operator-=(vec3 v);

    /**
     * @brief calculate the dot product of two vec3s
     * 
     * @param v the other vec3
     * @return float the dot product of this vec3 and the other vec3
     */
    float operator*(vec3 v);

    /**
     * @brief scale the vec3 by an float
     * 
     * @param v the float to scale with
     * @return vec3 the scaled vector
     */
    vec3 operator*(float v);

    /**
     * @brief scale this vec3 with another vec3
     * 
     * @param v the other vector to scale this vector with
     */
    void operator*=(vec3 v);

    /**
     * @brief scale the vector with another vec3
     * 
     * @param v the INVERSE of the other vector to scale with
     * @return vec3 the scale of this vec3 and the inverse of the other vec3
     */
    vec3 operator/(vec3 v);

    /**
     * @brief scale this vec3 with another vec3
     * 
     * @param v the INVERSE of the other vec3 to scale with
     */
    void operator/=(vec3 v);

    /**
     * @brief check if two vec3s are equal
     * 
     * @param v the vec3 to compare with
     * @return true : both vec3s are equal | 
     * @return false : the vec3s are not equal
     */
    bool operator==(vec3 v);

    /**
     * @brief check if two vec3s are not equal
     * 
     * @param v the vec3 to compare with
     * @return true : both vec3s are not equal | 
     * @return false : the vec3s are equal
     */
    bool operator!=(vec3 v);

    /**
     * @brief check if an vec3 is smaller or equal than another vec3
     * 
     * @param v the vec3 that should be equal or less than this vec3
     * @return true : the vec3 is less or equal than this vec3 | 
     * @return false : the other vec3 is larger than this vec3
     */
    bool operator>=(vec3 v);

    /**
     * @brief check if an vec3 is larger or equal than another vec3
     * 
     * @param v the vec3 that should be equal or larger than this vec3
     * @return true : the vec3 is larger or equal than this vec3 | 
     * @return false : the other vec3 is smaler than this vec3
     */
    bool operator<=(vec3 v);

    /**
     * @brief check if an vec3 is larger than another vec3
     * 
     * @param v the vec3 that should be larger than this vec3
     * @return true : the vec3 is larger than this vec3 | 
     * @return false : the other vec3 is equal or smaler than this vec3
     */
    bool operator<(vec3 v);

    /**
     * @brief check if an vec3 is larger than another vec3
     * 
     * @param v the vec3 that should be equal than this vec3
     * @return true : the vec3 is larger than this vec3 | 
     * @return false : the other vec3 is equal or smaler than this vec3
     */
    bool operator>(vec3 v);

    /**
     * @brief calculate the length of this vec3
     * 
     * @return float the length of the vector
     */
    float length();

    /**
     * @brief normalize this vec3
     */
    void normalize();

    /**
     * @brief scale this vec3 with another one
     * 
     * @param v the other vec3
     * @return vec3 this vector scaled with the other one
     */
    vec3 scale(vec3 v);

    /**
     * @brief calculate the cross product of this vector and another vector
     * 
     * @param v the other vector for the cross product
     * @return vec3 the cross product
     */
    vec3 cross(vec3 v);
};

#endif