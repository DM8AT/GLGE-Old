#ifndef _CMLMAT4_H_
#define _CMLMAT4_H_
#pragma once
#include "CMLVec4.h"

/**
 * @brief a 4*4 matrix of floates, like mat3, but with an extra row and collum
 * 
 */
class mat4
{
public:
    float m[4][4] = {
        {1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}
    };

    /**
     * @brief default constructor
     * 
     */
    mat4();

    /**
     * @brief Construct a new mat4
     * 
     * @param m00 the value for matrix value at position 0,0
     * @param m01 the value for matrix value at position 0,1
     * @param m02 the value for matrix value at position 0,2
     * @param m03 the value for matrix value at position 0,3
     * @param m10 the value for matrix value at position 1,0
     * @param m11 the value for matrix value at position 1,1
     * @param m12 the value for matrix value at position 1,2
     * @param m13 the value for matrix value at position 1,3
     * @param m20 the value for matrix value at position 2,0
     * @param m21 the value for matrix value at position 2,1
     * @param m22 the value for matrix value at position 2,2
     * @param m23 the value for matrix value at position 2,3
     * @param m30 the value for matrix value at position 3,0
     * @param m31 the value for matrix value at position 3,1
     * @param m32 the value for matrix value at position 3,2
     * @param m33 the value for matrix value at position 3,3
     */
    mat4(float m00, float m01, float m02, float m03,
         float m10, float m11, float m12, float m13,
         float m20, float m21, float m22, float m23,
         float m30, float m31, float m32, float m33);

    /**
     * @brief Construct a new mat4 from an float array
     * 
     * @param arr the float array to use
     */
    mat4(float arr[16]);

    /**
     * @brief add another matrix to this matrix
     * 
     * @param c the other matrix to add to this matrix
     * @return mat4 the sum of the two matrices
     */
    mat4 operator+(mat4 c);
    /**
     * @brief add an scalar to this matrix
     * 
     * @param c the scalar to add to all values in this matrix
     * @return mat4 the sum of the scalar and the matrix
     */
    mat4 operator+(float c);
    /**
     * @brief add another matrix to this matrix
     * 
     * @param c the matrix to add to this matrix
     */
    void operator+=(mat4 c);
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
     * @return mat4 the difference of the two matrices
     */
    mat4 operator-(mat4 c);
    /**
     * @brief subtract an scalar from this matrix
     * 
     * @param c the scalar to subtract from all values in this matrix
     * @return mat4 the difference of the scalar and the matrix
     */
    mat4 operator-(float c);
    /**
     * @brief subtract another matrix from this matrix
     * 
     * @param c the matrix to subtract from this matrix
     */
    void operator-=(mat4 c);
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
     * @return mat4 the product of the two matrices
     */
    mat4 operator*(mat4 c);
    /**
     * @brief scale the matrix with an scalar
     * 
     * @param s the scalar to scale the matrix with
     * @return mat4 the scaled matrix
     */
    mat4 operator*(float s);
    /**
     * @brief multiply an vector with an vec4
     * 
     * @param v the vector to multiply with
     * @return vec4 the product of the matrix and the vector
     */
    vec4 operator*(vec4 v);
    /**
     * @brief multiply this matrix with another matrix
     * 
     * @param c the other matrix
     */
    void operator*=(mat4 c);
    /**
     * @brief scale this matrix with another matrix
     * 
     * @param s the scalr to scale with
     */
    void operator*=(float s);
};

#endif