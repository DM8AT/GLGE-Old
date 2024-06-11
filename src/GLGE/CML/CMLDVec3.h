//check if vec to vec casts should be possible
#ifndef CML_NO_VEC_CASTS
#include "CMLDVec2.h"
#endif

#ifndef _CMLdvec3_H_
#define _CMLdvec3_H_

/**
 * @brief like vec3 but with larger percission
 * 
 */
class dvec3
{
public:
    /**
     * @brief the x component of the vector of doulbes
     */
    double x,
    /**
     * @brief the y component of the vector of doubles
     */
    y,
    /**
     * @brief the z component of the vector of doubles
     */
    z;

    /**
     * @brief default constructor
     */
    dvec3();

    /**
     * @brief Construct a new dvec3
     * 
     * @param xyz the value for all 3 axis
     */
    dvec3(double xyz);

    /**
     * @brief Construct a new dvec3
     * 
     * @param x the value for the x axis
     * @param y the value for the y axis
     * @param z the value for the z axis
     */
    dvec3(double x, double y, double z);

#ifndef CML_NO_VEC_CASTS
    /**
     * @brief Construct a new dvec3
     * 
     * @param xy the value for the x and y axis
     * @param z the value for the z axis
     */
    dvec3(dvec2 xy, double z);

    /**
     * @brief Construct a new dvec3
     * 
     * @param x the value for the x axis
     * @param yz the value for the y and z axis
     */
    dvec3(double x, dvec2 yz);
#endif

    /**
     * @brief add two dvec3s
     * 
     * @param v the other dvec3 that should be added with this vector
     * @return dvec3 the sum of the vectors
     */
    dvec3 operator+(dvec3 v);

    /**
     * @brief add a dvec3 to this dvec3
     * 
     * @param v the dvec3 that should be added to this dvec3
     */
    void operator+=(dvec3 v);

    /**
     * @brief subtract two dvec3s
     * 
     * @param v the dvec3 to subtract from this dvec3
     * @return dvec3 the difference of this dvec3 and the other devec2
     */
    dvec3 operator-(dvec3 v);

    /**
     * @brief subtract an other dvec3 from this dvec3
     * 
     * @param v the other dvec3
     */
    void operator-=(dvec3 v);

    /**
     * @brief calculate the dot product of two dvec3s
     * 
     * @param v the other dvec3
     * @return double the dot product of this dvec3 and the other dvec3
     */
    double operator*(dvec3 v);

    /**
     * @brief scale this dvec3 with another dvec3
     * 
     * @param v the other vector to scale this vector with
     */
    void operator*=(dvec3 v);

    /**
     * @brief scale the vector with another dvec3
     * 
     * @param v the INVERSE of the other vector to scale with
     * @return dvec3 the scale of this dvec3 and the inverse of the other dvec3
     */
    dvec3 operator/(dvec3 v);

    /**
     * @brief scale this dvec3 with another dvec3
     * 
     * @param v the INVERSE of the other dvec3 to scale with
     */
    void operator/=(dvec3 v);

    /**
     * @brief check if two dvec3s are equal
     * 
     * @param v the dvec3 to compare with
     * @return true : both dvec3s are equal | 
     * @return false : the dvec3s are not equal
     */
    bool operator==(dvec3 v);

    /**
     * @brief check if two dvec3s not are equal
     * 
     * @param v the dvec3 to compare with
     * @return true : both dvec3s are not equal | 
     * @return false : the dvec3s are equal
     */
    bool operator!=(dvec3 v);

    /**
     * @brief check if an dvec3 is smaller or equal than another dvec3
     * 
     * @param v the dvec3 that should be equal or less than this dvec3
     * @return true : the dvec3 is less or equal than this dvec3 | 
     * @return false : the other dvec3 is larger than this dvec3
     */
    bool operator>=(dvec3 v);

    /**
     * @brief check if an dvec3 is larger or equal than another dvec3
     * 
     * @param v the dvec3 that should be equal or larger than this dvec3
     * @return true : the dvec3 is larger or equal than this dvec3 | 
     * @return false : the other dvec3 is smaler than this dvec3
     */
    bool operator<=(dvec3 v);

    /**
     * @brief check if an dvec3 is larger than another dvec3
     * 
     * @param v the dvec3 that should be larger than this dvec3
     * @return true : the dvec3 is larger than this dvec3 | 
     * @return false : the other dvec3 is equal or smaler than this dvec3
     */
    bool operator<(dvec3 v);

    /**
     * @brief check if an dvec3 is larger than another dvec3
     * 
     * @param v the dvec3 that should be equal than this dvec3
     * @return true : the dvec3 is larger than this dvec3 | 
     * @return false : the other dvec3 is equal or smaler than this dvec3
     */
    bool operator>(dvec3 v);

    /**
     * @brief calculate the length of this dvec3
     * 
     * @return double the length of the vector
     */
    double length();

    /**
     * @brief normalize this dvec3
     */
    void normalize();

    /**
     * @brief scale this dvec3 with another one
     * 
     * @param v the other dvec3
     * @return dvec3 this vector scaled with the other one
     */
    dvec3 scale(dvec3 v);

    /**
     * @brief calculate the cross product of this vector and another vector
     * 
     * @param v the other vector for the cross product
     * @return vec3 the cross product
     */
    dvec3 cross(dvec3 v);
};

#endif