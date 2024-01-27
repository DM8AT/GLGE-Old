#ifndef CML_NO_VEC_CASTS
#include "CMLVec2.h"
#include "CMLVec3.h"
#endif

#ifndef _CMLVEC4_H_
#define _CMLVEC4_H_

/**
 * @brief a simple 4D vector
 * 
 */
class vec4
{
public:
    float x,y,z,w;

    /**
     * @brief default constructor
     * 
     */
    vec4();

    /**
     * @brief Construct a new vec4
     * 
     * @param xyzw the value for all 4 axis
     */
    vec4(float xyzw);

    /**
     * @brief Construct a new vec4
     * 
     * @param x the value for the x axis
     * @param y the value for the y axis
     * @param z the value for the z axis
     * @param w the value for the z axis
     */
    vec4(float x, float y, float z, float w);

#ifndef CML_NO_VEC_CASTS
    /**
     * @brief Construct a new vec4
     * 
     * @param xyz the value for the x, y and z axis
     * @param w the value for the z axis
     */
    vec4(vec3 xyz, float w);

    /**
     * @brief Construct a new vec4
     * 
     * @param x the value for the x axis
     * @param yzw the value for the y,z and w axis
     */
    vec4(float x, vec3 yzw);

    /**
     * @brief Construct a new vec4
     * 
     * @param xy the value for the x and y axis
     * @param z the value for the z axis
     * @param w the value for the w axis
     */
    vec4(vec2 xy, float z, float w);

    /**
     * @brief Construct a new vec4
     * 
     * @param x the value for the x axis
     * @param yz the value for the y and z axis
     * @param w the value for the w axis
     */
    vec4(float x, vec2 yz, float w);

    /**
     * @brief Construct a new vec4
     * 
     * @param x the value for the x axis
     * @param y the value for the y axis
     * @param zw the value for the z and w axis
     */
    vec4(float x, float y, vec2 zw);

    /**
     * @brief Construct a new vec4
     * 
     * @param xy the value for the x and y axis
     * @param zw the value for the z and w axis
     */
    vec4(vec2 xy, vec2 zw);
#endif

    /**
     * @brief add two vec4s
     * 
     * @param v the other vec4 that should be added with this vector
     * @return vec4 the sum of the vectors
     */
    vec4 operator+(vec4 v);

    /**
     * @brief add a vec4 to this vec4
     * 
     * @param v the vec4 that should be added to this vec4
     */
    void operator+=(vec4 v);

    /**
     * @brief subtract two vec4s
     * 
     * @param v the vec4 to subtract from this vec4
     * @return vec4 the difference of this vec4 and the other devec2
     */
    vec4 operator-(vec4 v);

    /**
     * @brief subtract an other vec4 from this vec4
     * 
     * @param v the other vec4
     */
    void operator-=(vec4 v);

    /**
     * @brief calculate the dot product of two vec4s
     * 
     * @param v the other vec4
     * @return float the dot product of this vec4 and the other vec4
     */
    float operator*(vec4 v);

    /**
     * @brief scale this vec4 with another vec4
     * 
     * @param v the other vector to scale this vector with
     */
    void operator*=(vec4 v);

    /**
     * @brief scale the vector with another vec4
     * 
     * @param v the INVERSE of the other vector to scale with
     * @return vec4 the scale of this vec4 and the inverse of the other vec4
     */
    vec4 operator/(vec4 v);

    /**
     * @brief scale this vec4 with another vec4
     * 
     * @param v the INVERSE of the other vec4 to scale with
     */
    void operator/=(vec4 v);

    /**
     * @brief check if two vec4s are equal
     * 
     * @param v the vec4 to compare with
     * @return true : both vec4s are equal | 
     * @return false : the vec4s are not equal
     */
    bool operator==(vec4 v);

    /**
     * @brief check if an vec4 is smaller or equal than another vec4
     * 
     * @param v the vec4 that should be equal or less than this vec4
     * @return true : the vec4 is less or equal than this vec4 | 
     * @return false : the other vec4 is larger than this vec4
     */
    bool operator>=(vec4 v);

    /**
     * @brief check if an vec4 is larger or equal than another vec4
     * 
     * @param v the vec4 that should be equal or larger than this vec4
     * @return true : the vec4 is larger or equal than this vec4 | 
     * @return false : the other vec4 is smaler than this vec4
     */
    bool operator<=(vec4 v);

    /**
     * @brief check if an vec4 is larger than another vec4
     * 
     * @param v the vec4 that should be larger than this vec4
     * @return true : the vec4 is larger than this vec4 | 
     * @return false : the other vec4 is equal or smaler than this vec4
     */
    bool operator<(vec4 v);

    /**
     * @brief check if an vec4 is larger than another vec4
     * 
     * @param v the vec4 that should be equal than this vec4
     * @return true : the vec4 is larger than this vec4 | 
     * @return false : the other vec4 is equal or smaler than this vec4
     */
    bool operator>(vec4 v);

    /**
     * @brief calculate the length of this vec4
     * 
     * @return float the length of the vector
     */
    float length();

    /**
     * @brief normalize this vec4
     */
    void normalize();

    /**
     * @brief scale this vec4 with another one
     * 
     * @param v the other vec4
     * @return vec4 this vector scaled with the other one
     */
    vec4 scale(vec4 v);
};

#endif