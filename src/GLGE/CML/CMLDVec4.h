#ifndef CML_NO_VEC_CASTS
#include "CMLDVec2.h"
#include "CMLDVec3.h"
#endif

#ifndef _CMLDVEC4_H_
#define _CMLDVEC4_H_

/**
 * @brief like vec4 but with larger percission
 */
class dvec4
{
public:
    double x,y,z,w;

    /**
     * @brief default constructor
     * 
     */
    dvec4();

    /**
     * @brief Construct a new dvec4
     * 
     * @param xyzw the value for all 4 axis
     */
    dvec4(double xyzw);

    /**
     * @brief Construct a new dvec4
     * 
     * @param x the value for the x axis
     * @param y the value for the y axis
     * @param z the value for the z axis
     * @param w the value for the z axis
     */
    dvec4(double x, double y, double z, double w);

#ifndef CML_NO_VEC_CASTS
    /**
     * @brief Construct a new dvec4
     * 
     * @param xyz the value for the x, y and z axis
     * @param w the value for the z axis
     */
    dvec4(dvec3 xyz, double w);

    /**
     * @brief Construct a new dvec4
     * 
     * @param x the value for the x axis
     * @param yzw the value for the y,z and w axis
     */
    dvec4(double x, dvec3 yzw);

    /**
     * @brief Construct a new dvec4
     * 
     * @param xy the value for the x and y axis
     * @param z the value for the z axis
     * @param w the value for the w axis
     */
    dvec4(dvec2 xy, double z, double w);

    /**
     * @brief Construct a new dvec4
     * 
     * @param x the value for the x axis
     * @param yz the value for the y and z axis
     * @param w the value for the w axis
     */
    dvec4(double x, dvec2 yz, double w);

    /**
     * @brief Construct a new dvec4
     * 
     * @param x the value for the x axis
     * @param y the value for the y axis
     * @param zw the value for the z and w axis
     */
    dvec4(double x, double y, dvec2 zw);

    /**
     * @brief Construct a new dvec4
     * 
     * @param xy the value for the x and y axis
     * @param zw the value for the z and w axis
     */
    dvec4(dvec2 xy, dvec2 zw);
#endif

    /**
     * @brief add two dvec4s
     * 
     * @param v the other dvec4 that should be added with this vector
     * @return dvec4 the sum of the vectors
     */
    dvec4 operator+(dvec4 v);

    /**
     * @brief add a dvec4 to this dvec4
     * 
     * @param v the dvec4 that should be added to this dvec4
     */
    void operator+=(dvec4 v);

    /**
     * @brief subtract two dvec4s
     * 
     * @param v the dvec4 to subtract from this dvec4
     * @return dvec4 the difference of this dvec4 and the other devec2
     */
    dvec4 operator-(dvec4 v);

    /**
     * @brief subtract an other dvec4 from this dvec4
     * 
     * @param v the other dvec4
     */
    void operator-=(dvec4 v);

    /**
     * @brief calculate the dot product of two dvec4s
     * 
     * @param v the other dvec4
     * @return double the dot product of this dvec4 and the other dvec4
     */
    double operator*(dvec4 v);

    /**
     * @brief scale this dvec4 with another dvec4
     * 
     * @param v the other vector to scale this vector with
     */
    void operator*=(dvec4 v);

    /**
     * @brief scale the vector with another dvec4
     * 
     * @param v the INVERSE of the other vector to scale with
     * @return dvec4 the scale of this dvec4 and the inverse of the other dvec4
     */
    dvec4 operator/(dvec4 v);

    /**
     * @brief scale this dvec4 with another dvec4
     * 
     * @param v the INVERSE of the other dvec4 to scale with
     */
    void operator/=(dvec4 v);

    /**
     * @brief check if two dvec4s are equal
     * 
     * @param v the dvec4 to compare with
     * @return true : both dvec4s are equal | 
     * @return false : the dvec4s are not equal
     */
    bool operator==(dvec4 v);

    /**
     * @brief check if an dvec4 is smaller or equal than another dvec4
     * 
     * @param v the dvec4 that should be equal or less than this dvec4
     * @return true : the dvec4 is less or equal than this dvec4 | 
     * @return false : the other dvec4 is larger than this dvec4
     */
    bool operator>=(dvec4 v);

    /**
     * @brief check if an dvec4 is larger or equal than another dvec4
     * 
     * @param v the dvec4 that should be equal or larger than this dvec4
     * @return true : the dvec4 is larger or equal than this dvec4 | 
     * @return false : the other dvec4 is smaler than this dvec4
     */
    bool operator<=(dvec4 v);

    /**
     * @brief check if an dvec4 is larger than another dvec4
     * 
     * @param v the dvec4 that should be larger than this dvec4
     * @return true : the dvec4 is larger than this dvec4 | 
     * @return false : the other dvec4 is equal or smaler than this dvec4
     */
    bool operator<(dvec4 v);

    /**
     * @brief check if an dvec4 is larger than another dvec4
     * 
     * @param v the dvec4 that should be equal than this dvec4
     * @return true : the dvec4 is larger than this dvec4 | 
     * @return false : the other dvec4 is equal or smaler than this dvec4
     */
    bool operator>(dvec4 v);

    /**
     * @brief calculate the length of this dvec4
     * 
     * @return double the length of the vector
     */
    double length();

    /**
     * @brief normalize this dvec4
     */
    void normalize();

    /**
     * @brief scale this dvec4 with another one
     * 
     * @param v the other dvec4
     * @return dvec4 this vector scaled with the other one
     */
    dvec4 scale(dvec4 v);
};

#endif