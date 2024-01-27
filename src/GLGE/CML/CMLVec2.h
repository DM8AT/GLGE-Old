#ifndef _CMLVEC2_H_
#define _CMLVEC2_H_
#pragma once

/**
 * @brief a simple 2D vector
 */
class vec2
{
public:
    float x,y;

    /**
     * @brief default construcotr
     */
    vec2();

    /**
     * @brief Construct a new vec2
     * 
     * @param xy the value for the x AND y component
     */
    vec2(float xy);

    /**
     * @brief Construct a new vec2
     * 
     * @param x the value for the x component
     * @param y the value for the y component
     */
    vec2(float x, float y);

    /**
     * @brief add two vec2s
     * 
     * @param v the other vec2 that should be added with this vector
     * @return vec2 the sum of the vectors
     */
    vec2 operator+(vec2 v);

    /**
     * @brief add a vec2 to this vec2
     * 
     * @param v the vec2 that should be added to this vec2
     */
    void operator+=(vec2 v);

    /**
     * @brief subtract two vec2s
     * 
     * @param v the vec2 to subtract from this vec2
     * @return vec2 the difference of this vec2 and the other devec2
     */
    vec2 operator-(vec2 v);

    /**
     * @brief subtract an other vec2 from this vec2
     * 
     * @param v the other vec2
     */
    void operator-=(vec2 v);

    /**
     * @brief calculate the dot product of two vec2s
     * 
     * @param v the other vec2
     * @return float the dot product of this vec2 and the other vec2
     */
    float operator*(vec2 v);

    /**
     * @brief scale the vec2 by an float
     * 
     * @param v the float to scale with
     * @return vec2 the scaled vector
     */
    vec2 operator*(float v);

    /**
     * @brief scale this vec2 with another vec2
     * 
     * @param v the other vector to scale this vector with
     */
    void operator*=(vec2 v);

    /**
     * @brief scale the vector with another vec2
     * 
     * @param v the INVERSE of the other vector to scale with
     * @return vec2 the scale of this vec2 and the inverse of the other vec2
     */
    vec2 operator/(vec2 v);

    /**
     * @brief scale this vec2 with another vec2
     * 
     * @param v the INVERSE of the other vec2 to scale with
     */
    void operator/=(vec2 v);

    /**
     * @brief check if two vec2s are equal
     * 
     * @param v the vec2 to compare with
     * @return true : both vec2s are equal | 
     * @return false : the vec2s are not equal
     */
    bool operator==(vec2 v);

    /**
     * @brief check if an vec2 is smaller or equal than another vec2
     * 
     * @param v the vec2 that should be equal or less than this vec2
     * @return true : the vec2 is less or equal than this vec2 | 
     * @return false : the other vec2 is larger than this vec2
     */
    bool operator>=(vec2 v);

    /**
     * @brief check if an vec2 is larger or equal than another vec2
     * 
     * @param v the vec2 that should be equal or larger than this vec2
     * @return true : the vec2 is larger or equal than this vec2 | 
     * @return false : the other vec2 is smaler than this vec2
     */
    bool operator<=(vec2 v);

    /**dvec4
     * @brief check if an vec2 is larger than another vec2
     * 
     * @param v the vec2 that should be larger than this vec2
     * @return true : the vec2 is larger than this vec2 | 
     * @return false : the other vec2 is equal or smaler than this vec2
     */
    bool operator<(vec2 v);

    /**
     * @brief check if an vec2 is larger than another vec2
     * 
     * @param v the vec2 that should be equal than this vec2
     * @return true : the vec2 is larger than this vec2 | 
     * @return false : the other vec2 is equal or smaler than this vec2
     */
    bool operator>(vec2 v);

    /**
     * @brief calculate the length of this vec2
     * 
     * @return float the length of the vector
     */
    float length();

    /**
     * @brief normalize this vec2
     */
    void normalize();

    /**
     * @brief scale this vec2 with another one
     * 
     * @param v the other vec2
     * @return vec2 this vector scaled with the other one
     */
    vec2 scale(vec2 v);
};

#endif