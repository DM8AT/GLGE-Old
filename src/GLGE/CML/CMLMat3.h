#ifndef _CMLMAT3_H_
#define _CMLMAT3_H_
#pragma once
#include "CMLVec3.h"

/**
 * @brief a 3*3 matrix of floates, like mat2, but with an extra row and collum
 */
class mat3
{
public:
    float m[3][3];

    /**
     * @brief default constructor
     * 
     */
    mat3();
    /**
     * @brief Construct a new mat3
     * 
     * @param m00 value for positoin 0,0 in matrix
     * @param m01 value for positoin 0,1 in matrix
     * @param m02 value for positoin 0,2 in matrix
     * @param m10 value for positoin 1,0 in matrix
     * @param m11 value for positoin 1,1 in matrix
     * @param m12 value for positoin 1,2 in matrix
     * @param m20 value for positoin 2,0 in matrix
     * @param m21 value for positoin 2,1 in matrix
     * @param m22 value for positoin 2,2 in matrix
     */
    mat3(float m00, float m01, float m02, 
         float m10, float m11, float m12,
         float m20, float m21, float m22);

    /**
     * @brief add another matrix to this matrix
     * 
     * @param c the other matrix to add to this matrix
     * @return mat3 the sum of the two matrices
     */
    mat3 operator+(mat3 c);
    /**
     * @brief add an scalar to this matrix
     * 
     * @param c the scalar to add to all values in this matrix
     * @return mat3 the sum of the scalar and the matrix
     */
    mat3 operator+(float c);
    /**
     * @brief add another matrix to this matrix
     * 
     * @param c the matrix to add to this matrix
     */
    void operator+=(mat3 c);
    /**
     * @brief add an scalar to all values in this matrix
     * 
     * @param c the scalar to add to all vlaues
     */
    void operator+=(float c);

    /**
     * @brief subtract another matrix from this matrix
     * 
     * @param c the matrix to subtract from this matrix
     * @return mat3 the difference of the two matrices
     */
    mat3 operator-(mat3 c);
    /**
     * @brief subtract an scalar from this matrix
     * 
     * @param c the scalar to subtract from all values in this matrix
     * @return mat3 the difference of the scalar and the matrix
     */
    mat3 operator-(float c);
    /**
     * @brief subtract another matrix from this matrix
     * 
     * @param c the matrix to subtract from this matrix
     */
    void operator-=(mat3 c);
    /**
     * @brief subtract an scalar from this matrix
     * 
     * @param c the scalar to subtract from all values in this matrix
     */
    void operator-=(float c);

    /**
     * @brief multiply this matrix with another matrix
     * 
     * @param c the matrix to multiply with
     * @return mat3 the product of the two matrices
     */
    mat3 operator*(mat3 c);
    /**
     * @brief scale the matrix with an scalar
     * 
     * @param s the scalar to scale the matrix with
     * @return mat3 the scaled matrix
     */
    mat3 operator*(float s);
    /**
     * @brief multiply an vector with an vec3
     * 
     * @param v the vector to multiply with
     * @return vec3 the product of the matrix and the vector
     */
    vec3 operator*(vec3 v);
    /**
     * @brief multiply this matrix with another matrix
     * 
     * @param c the other matrix
     */
    void operator*=(mat3 c);
    /**
     * @brief scale this matrix with another matrix
     * 
     * @param s the scalr to scale with
     */
    void operator*=(float s);
};

#endif