#include "CMLVec3.h"

#ifndef _CMLQUATERNION_H_
#define _CMLQUATERNION_H_
#pragma once

/**
 * @brief a quaternion used for camera rotation
 * 
 */
class Quaternion
{
public:
    float w,x,y,z;

    /**
     * @brief default constructor
     * 
     */
    Quaternion();

    /**
     * @brief Construct a new Quaternion
     * 
     * @param w the value for the w or the angle component
     * @param x the value for the x component
     * @param y the value for the y component
     * @param z the value for the z component
     */
    Quaternion(float w, float x, float y, float z);

    /**
     * @brief Construct a new Quaternion
     * 
     * @param Angle the value for the angle
     * @param vector the vector for the x,y and z componenten of the quaternion
     */
    Quaternion(float Angle, vec3 vector);

    /**
     * @brief Destroy the Quaternion
     * 
     */
    ~Quaternion();

    /**
     * @brief sum two quaternions together
     * 
     * @param q the other quaternion to sum together
     * @return Quaternion the sum of this and the other quaternion
     */
    Quaternion operator+(Quaternion q);
    /**
     * @brief add another quaternion to this quaternion
     * 
     * @param q the quaternion to add to this quaternion
     */
    void operator+=(Quaternion q);

    /**
     * @brief subtract two quaternions
     * 
     * @param q the quaternion to subtract from this quaternion
     * @return Quaternion the difference ot this and the other quaternion
     */
    Quaternion operator-(Quaternion q);
    /**
     * @brief subtract another quaternion from this quaternion
     * 
     * @param q the other quaternion to subtract from this quaternion
     */
    void operator-=(Quaternion q);

    /**
     * @brief multiply two quaternions
     * 
     * @param q the other quaternion to multiply with
     * @return Quaternion the product of the two quaternions
     */
    Quaternion operator*(Quaternion q);
    /**
     * @brief multiply this quaternion with an vec3
     * 
     * @param v the vec3 to multiply with
     * @return Quaternion the product of the quaternion and vec3
     */
    Quaternion operator*(vec3 v);
    /**
     * @brief multiply this quaternion with another quaternion
     * 
     * @param q the other quaternion to multiply with
     */
    void operator*=(Quaternion q);

    /**
     * @brief divide two quaternions
     * 
     * @param q the other quaternion to divide with
     * @return Quaternion resault of the division
     */
    Quaternion operator/(Quaternion q);
    /**
     * @brief divide this quaternion with another one
     * 
     * @param q the other quaternion to divide with
     */
    void operator/=(Quaternion q);

    /**
     * @brief divide a quaternion and a float
     * 
     * @param f the float to divide with
     * @return Quaternion the resault of the division
     */
    Quaternion operator/(float f);
    /**
     * @brief divide this quaternion with an float
     * 
     * @param f the float to divide with
     */
    void operator/=(float f);

    /**
     * @brief conjugate this quaternion and don't change this
     * 
     * @return Quaternion the conjugated quaternion
     */
    Quaternion conjugate();
    /**
     * @brief conjuage this quaternion to this
     * 
     */
    void conjugateThis();

    /**
     * @brief get the length of the quaternion
     * 
     * @return float the length of the quaternion
     */
    float length();

    /**
     * @brief normal the quaternion and don't change this
     * 
     * @return Quaternion the normal quaternion
     */
    Quaternion normal();
    /**
     * @brief normal this quaternion and change this
     * 
     */
    void normalizeThis();

    /**
     * @brief invert this quaternion and don't change this
     * 
     * @return Quaternion the inverted quaternion
     */
    Quaternion invert();
    /**
     * @brief invert this quaternion and change this
     * 
     */
    void invertThis();
};

#endif